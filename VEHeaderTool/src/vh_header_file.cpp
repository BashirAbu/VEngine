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

		std::function <void(nlohmann::ordered_json& enumJson, Enum& _enum)> enumDetails = [&](nlohmann::ordered_json& enumJson, Enum& _enum)
			{
				_enum.name = enumJson["name"];

				for (auto itr = enumJson["meta"].begin(); itr != enumJson["meta"].end(); itr++)
				{
					Meta meta = {};
					meta.key = itr.key();
					if (!itr.value().is_null())
					{
						meta.value = itr.value();
					}
					_enum.meta.push_back(meta);
				}

				//items
				for (auto member : enumJson["members"])
				{
					_enum.items.push_back(member["key"]);
				}

			};

		std::function <void(nlohmann::ordered_json& classJson, Component& comp)> componentDetails;

		componentDetails = [&](nlohmann::ordered_json& classJson, Component& comp)
			{
			
				comp.name.name = classJson["name"];
				
				for (auto itr = classJson["meta"].begin(); itr != classJson["meta"].end(); itr++)
				{
					Meta meta = {};
					meta.key = itr.key();
					if (!itr.value().is_null())
					{
						meta.value = itr.value();
					}
					comp.name.meta.push_back(meta);
				}

				for (auto member : classJson["members"])
				{
					ParsedName prop;
					for (auto itr = member["meta"].begin(); itr != member["meta"].end(); itr++)
					{
						Meta meta = {};
						meta.key = itr.key();
						if (!itr.value().is_null())
						{
							meta.value = itr.value();
						}
						prop.meta.push_back(meta);
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

					if (member["dataType"].contains("type"))
					{
						if (member["dataType"]["type"] == "template")
						{
							prop.templateType = true;
							if (member["dataType"].contains("arguments") && prop.templateType)
							{
								for (auto arg : member["dataType"]["arguments"])
								{
									prop.args.push_back(arg["name"]);
								}
							}
						}
					}
					comp.properites.push_back(prop);
				}
			};


		std::function <void(nlohmann::ordered_json& systemJson, System& system)> systemDetails;
		systemDetails = [&](nlohmann::ordered_json& systemJson, System& system)
			{
				system.name = systemJson["name"];
				for (auto itr = systemJson["meta"].begin(); itr != systemJson["meta"].end(); itr++)
				{
					Meta meta = {};
					meta.key = itr.key();
					if (!itr.value().is_null())
					{
						meta.value = itr.value();
					}
					system.meta.push_back(meta);
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


		std::function <void(nlohmann::ordered_json& callBackJson, Callback& callback)> callBackDetails;

		callBackDetails = [&](nlohmann::ordered_json& callBackJson, Callback& callback)
			{
				callback.name = callBackJson["name"];
				for (auto itr = callBackJson["meta"].begin(); itr != callBackJson["meta"].end(); itr++)
				{
					Meta meta = {};
					meta.key = itr.key();
					if (!itr.value().is_null())
					{
						meta.value = itr.value();
					}
					callback.meta.push_back(meta);
				}
			};


		std::function <void(nlohmann::ordered_json& elenJson, bool component)> recursiveNamespaces;

		std::vector<std::string> nameSpaces;
		recursiveNamespaces = [&](nlohmann::ordered_json& elenJson, bool isComponentOrEnum)
			{
				if (elenJson["type"] == "namespace")
				{
					nameSpaces.push_back(elenJson["name"]);
					if (elenJson.contains("members"))
					{
						for (auto member : elenJson["members"])
						{
							if (member["type"] == "class" && isComponentOrEnum)
							{
								for (auto itr = member["meta"].begin(); itr != member["meta"].end(); itr++)
								{
									if (itr.key() == "Component")
									{
										Component comp;
										comp.name.nameSpaces = nameSpaces;
										componentDetails(member, comp);
										components.push_back(comp);
									}
								}
							}
							else if (member["type"] == "enum" && isComponentOrEnum)
							{
								Enum _enum = {};
								_enum.nameSpaces = nameSpaces;
								enumDetails(member, _enum);
								enums.push_back(_enum);
							}
							else if (member["type"] == "function" && !isComponentOrEnum)
							{
								for (auto itr = member["meta"].begin(); itr != member["meta"].end(); itr++)
								{
									if (itr.key() == "System")
									{
										System system;
										system.nameSpaces = nameSpaces;
										systemDetails(member, system);
										systems.push_back(system);
										
									}

									else if (itr.key() == "Callback") 
									{
										Callback callback;
										callback.nameSpaces = nameSpaces;
										callBackDetails(member, callback);
										callbacks.push_back(callback);
									}
								}
									
							}
							else if (member["type"] == "namespace")
							{
								recursiveNamespaces(member, isComponentOrEnum);
							}
						}
						nameSpaces.pop_back();
					}
					
				}
				else
				{
					if (elenJson["type"] == "class" && isComponentOrEnum)
					{
						for (auto itr = elenJson["meta"].begin(); itr != elenJson["meta"].end(); itr++)
						{
							if (itr.key() == "Component")
							{
								Component comp;
								comp.name.nameSpaces = nameSpaces;
								componentDetails(elenJson, comp);
								components.push_back(comp);
							}
						}

					}
					else if (elenJson["type"] == "enum" && isComponentOrEnum)
					{
						Enum _enum = {};
						_enum.nameSpaces = nameSpaces;
						enumDetails(elenJson, _enum);
						enums.push_back(_enum);
					}
					else if (elenJson["type"] == "function" && !isComponentOrEnum)
					{
						for (auto itr = elenJson["meta"].begin(); itr != elenJson["meta"].end(); itr++)
						{
							if (itr.key() == "System")
							{
								System system;
								system.nameSpaces = nameSpaces;
								systemDetails(elenJson, system);
								systems.push_back(system);
							}

							else if (itr.key() == "Callback")
							{
								Callback callback;
								callback.nameSpaces = nameSpaces;
								callBackDetails(elenJson, callback);
								callbacks.push_back(callback);

							}
						}
					}
				}
			};
		//comps			
		for (auto elementJson : fileJson)
		{
			recursiveNamespaces(elementJson, true);
		}
		nameSpaces.clear();
		//systems
		for (auto elementJson : fileJson)
		{
			recursiveNamespaces(elementJson, false);
		}
	}
}