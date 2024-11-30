#include "vh_header_file.h"
#include <nlohmann/json.hpp>

namespace VH
{
	HeaderFile::HeaderFile()
	{
	}
	HeaderFile::~HeaderFile()
	{
	}
	void HeaderFile::Prase()
	{
		nlohmann::ordered_json fileJson = nlohmann::ordered_json::parse(json);

		

		std::function <void(nlohmann::ordered_json& classJson, Component& comp)> componentDetails;

		componentDetails = [&](nlohmann::ordered_json& classJson, Component& comp)
			{
			
				comp.name.name = classJson["name"];
				
				for (auto itr = classJson["meta"].begin(); itr != classJson["meta"].end(); itr++)
				{
					comp.name.meta.push_back(itr.key());
				}

				for (auto member : classJson["members"])
				{
					ParsedName prop;
					for (auto itr = member["meta"].begin(); itr != member["meta"].end(); itr++)
					{
						prop.meta.push_back(itr.key());
					}
					
					std::string propertyNamespace;
					std::string propertyDatatype;
					std::string full = member["dataType"]["name"];

					size_t pos = full.rfind("::");

					propertyNamespace = (pos == std::string::npos) ? "" : full.substr(0, pos);
					propertyDatatype = (pos == std::string::npos)? full : full.substr(pos + 2);

					prop.nameSpaces.push_back(propertyNamespace);
					prop.dataType = propertyDatatype;
					prop.name = member["name"];

					comp.properites.push_back(prop);
				}
			};


		std::function <void(nlohmann::ordered_json& systemJson, System& system)> systemDetails;
		systemDetails = [&](nlohmann::ordered_json& systemJson, System& system)
			{
				system.name = systemJson["name"];
				for (auto itr = systemJson["meta"].begin(); itr != systemJson["meta"].end(); itr++)
				{
					system.meta.push_back(itr.key());
				}
				
				for (auto argsJson : systemJson["arguments"])
				{
					std::string type;

					if (argsJson["type"].contains("baseType"))
					{
						type = argsJson["type"]["baseType"]["name"];
					}
					else 
					{
						type = argsJson["type"]["name"];
					}
					if (type == "entity" || type == "flecs::entity")
					{
						continue;
					}
					ParsedName component;
					for (auto comp : this->components)
					{
						if (type.find(comp.name.name) != std::string::npos)
						{
							component = comp.name;
						}
					}

					if (component.name.empty())
					{
						component.name = type;
					}

					system.components.push_back(component);
				}
			};

		std::function <void(nlohmann::ordered_json& elenJson, bool component)> recursiveNamespaces;

		std::vector<std::string> nameSpaces;
		recursiveNamespaces = [&](nlohmann::ordered_json& elenJson, bool isComponent)
			{
				if (elenJson["type"] == "namespace")
				{
					nameSpaces.push_back(elenJson["name"]);
					if (elenJson.contains("members"))
					{
						for (auto member : elenJson["members"])
						{
							if (member["type"] == "class" && isComponent)
							{
								Component comp;
								comp.name.nameSpaces = nameSpaces;
								componentDetails(member, comp);
								components.push_back(comp);
								
							}
							else if (member["type"] == "function" && !isComponent)
							{
								System system;
								system.nameSpaces = nameSpaces;
								systemDetails(member, system);
								systems.push_back(system);
							}
							else if (member["type"] == "namespace")
							{
								recursiveNamespaces(member, isComponent);
							}
						}
						nameSpaces.pop_back();
					}
					else 
					{
						if (elenJson["type"] == "class" && isComponent)
						{
							Component comp;
							comp.name.nameSpaces = nameSpaces;
							componentDetails(elenJson, comp);
							components.push_back(comp);

						}
						else if (elenJson["type"] == "function" && !isComponent)
						{
							System system;
							system.nameSpaces = nameSpaces;
							systemDetails(elenJson, system);
							systems.push_back(system);
						}
					}
				}
			};
					
		for (auto elementJson : fileJson)
		{
			recursiveNamespaces(elementJson, true);
		}
		nameSpaces.clear();
		for (auto elementJson : fileJson)
		{
			recursiveNamespaces(elementJson, false);
		}
	}
}