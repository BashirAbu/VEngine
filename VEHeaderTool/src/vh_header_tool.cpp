#include "vh_header_tool.h"
#include <fstream>
#include <iostream>
namespace VH 
{
	HeaderTool::HeaderTool(Type type)
	{
		this->type = type;
	}
	HeaderTool::~HeaderTool()
	{
	}
	void HeaderTool::GenerateEditorUIFile()
	{
		std::string filename = "ve_generated_editor_file.cpp";

	}
	void HeaderTool::GenerateSerializationFile()
	{
		std::string filename = "ve_generated_serialization_file.cpp";

		std::string cppSourcefile= "#include \"ve_defines.h\"\n#include \"scenes/ve_scene.h\"\n";
		
		for (const HeaderFile& headerFile : headerFiles)
		{
			cppSourcefile += "#include \"" + headerFile.headerFilepath.generic_string() + "\"\n";
		}

		for (const HeaderFile& headerFile : headerFiles)
		{
			for (const Component& comp : headerFile.components)
			{
				std::string compFullSpacename = "";
				for (std::string sn : comp.name.nameSpaces)
				{
					compFullSpacename += sn + "::";
				}

				if (!comp.properites.empty())
				{


					cppSourcefile += "void Serialize_" + comp.name.name + "()\n"
						"{\n"
						"	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)"
						"	{\n"
						"		if(!strcmp(e.name().c_str(), \"" + comp.name.name + "\"))\n"
						"			return e;\n"
						"		return flecs::entity();"
						"	}); \n"
						"	if(compEntity)\n"
						"	{\n"
						"		flecs::component<" + compFullSpacename + comp.name.name + ">* comp = (flecs::component<" + compFullSpacename + comp.name.name + ">*)&compEntity; \n"
						"		comp->opaque(comp->world().component()";
					for (const ParsedName& property : comp.properites)
					{
						cppSourcefile += ".member<" + ((property.nameSpaces[0].empty()) ? "" : property.nameSpaces[0] + "::");
						cppSourcefile += property.dataType + ">(\"" + property.name + "\")";
					}
					cppSourcefile += ")\n"
						"		.serialize([](const flecs::serializer* s, const " + compFullSpacename + comp.name.name + "* data) -> int"
						"		{\n";
					for (const ParsedName& property : comp.properites)
					{
						cppSourcefile += "		s->member(\"" + property.name + "\");\n"
							"		s->value(data->" + property.name + ");\n";
					}
					cppSourcefile += "			 return 0;\n"
						"		}).ensure_member([](" + compFullSpacename + comp.name.name + "* data, const char* member) -> void*\n"
						"		{\n"
						"			if(0){ return nullptr;}\n";
					for (const ParsedName& property : comp.properites)
					{
						cppSourcefile += "			else if (!strcmp(member, \"" + property.name + "\")) { return &data->" + property.name + ";}\n";
					}
					cppSourcefile += "		return nullptr;\n"
						"		});\n"
						"	}\n"
						"}\n\n\n";

				}

				
			}
		}
		if (type == Type::Engine)
		{
			cppSourcefile += "void EngineGeneratedSerialization()\n";
		}
		else if (type == Type::Project)
		{
			cppSourcefile += "void ProjectGeneratedSerialization()\n";
		}
		cppSourcefile += "{\n";
		for (const HeaderFile& headerFile : headerFiles)
			for (const Component& comp : headerFile.components)
			{
				if (!comp.properites.empty())
				{
					cppSourcefile += "	 Serialize_" + comp.name.name + "();\n";
				}
			}
		cppSourcefile += "}\n\n\n\n\n";

		if (type == Type::Engine)
		{
			cppSourcefile += "void EngineGeneratedRegistration()\n";
		}
		else if (type == Type::Project)
		{
			cppSourcefile += "void ProjectGeneratedRegistration()\n";
		}
		cppSourcefile +=  "{\n";
			for (const HeaderFile& headerFile : headerFiles)
				for (const Component& comp : headerFile.components)
				{
					std::string compFullSpacename = "";
					for (std::string sn : comp.name.nameSpaces)
					{
						compFullSpacename += sn + "::";
					}
					cppSourcefile += "	 VE::Scene::GetSingleton()->GetFlecsWorld().component<" + compFullSpacename + comp.name.name + ">();\n";
					
				}

			//registere systems brother



		cppSourcefile +="}\n";


		std::cout << cppSourcefile << std::endl;

		std::ofstream output(generatedFilepath / filename);
		output << cppSourcefile;
		output.close();
	}
	void HeaderTool::Prase()
	{
		for (auto& hFile : headerFiles)
		{
			hFile.Prase();
		}
	}
}