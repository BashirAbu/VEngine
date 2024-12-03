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

	std::string FormatName(std::string input) 
	{
		std::string result;

		for (size_t i = 0; i < input.length(); ++i) {
			// Capitalize the first letter of the string
			if (i == 0) {
				result += std::toupper(input[i]);
			}
			// Add a space before uppercase letters (if not the first character)
			else if (std::isupper(input[i]) && std::islower(input[i - 1])) {
				result += ' ';
				result += input[i];
			}
			// Add a space if there's a transition like "Man" in "worldMan"
			else if (std::isupper(input[i]) && std::islower(input[i - 1])) {
				result += ' ';
				result += input[i];
			}
			else {
				result += input[i];
			}
		}

		return result;
	}

	void HeaderTool::EditorElement(std::string& cppSourefile, std::string compPTR, std::string dataType, std::string name)
	{
		std::string formatedName = FormatName(name);
		if (dataType.find("vec2") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Vec2(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("vec3") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Vec3(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("vec4") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Vec4(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("NormalizedColor") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Color(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		
		//VE_API void String(float& variable, std::string label);
		//VE_API void FileSystem(std::filesystem::path & path, std::string label);
		//VE_API void Checkbox(bool& variable, std::string label);
		else if (dataType.find("float") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Float(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("double") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Double(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("int") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Int(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("string") != std::string::npos)
		{
			cppSourefile += "			EditorElement::String(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("path") != std::string::npos)
		{
			cppSourefile += "			EditorElement::FileSystem(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}
		else if (dataType.find("bool") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Checkbox(" + compPTR + "->" + name + ", \"" + formatedName + "\");";
		}

		cppSourefile += "\n\n";

	}
	void HeaderTool::GenerateEditorUIFile()
	{
		std::string filename = "ve_generated_editor_file.cpp";

		std::string cppSourcefile = "#include \"ve_defines.h\"\n#include \"scenes/ve_scene.h\"\n#include \"editor/ve_editor.h\"\n#include <imgui.h>\n#include \"editor/ve_editor_elements.h\"\n\n";

		for (const HeaderFile& headerFile : headerFiles)
		{
			cppSourcefile += "#include \"" + headerFile.headerFilepath.generic_string() + "\"\n\n";
		}

		cppSourcefile += "using namespace VE;\n";
		cppSourcefile += "using namespace VE::Components;\n";
		cppSourcefile += "using namespace VE::_Components;\n\n\n";

		cppSourcefile += "void VE::Editor::DrawComponentElements(std::string name, flecs::entity entity)\n{\n";
		
		cppSourcefile += "	ImGui::PushID(name.c_str());\n"
			"	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;\n";


		for (const HeaderFile& headerFile : headerFiles)
		{
			for (const Component& comp : headerFile.components)
			{
				std::string compFullSpacename = "";
				for (std::string sn : comp.name.nameSpaces)
				{
					compFullSpacename += sn + "::";
				}

				//if (ImGui::BeginPopupContextItem(0, 1))
				//	//		{
				//	//			if (ImGui::MenuItem("Remove"))
				//	//			{
				//	//				entity.remove<VE::Components::SpriteComponent>();
				//	//				open = false;
				//	//			}
				//	//			ImGui::EndPopup();
				//	//		}

				cppSourcefile += "	if (name == \"" + comp.name.name + "\")\n	{\n"
					"		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());\n";
				cppSourcefile += "		if (open)\n		{\n"
					"			\n";
				cppSourcefile += "			" + compFullSpacename + comp.name.name + "* " + comp.name.name + "_ = entity.get_mut<" + compFullSpacename + comp.name.name + ">();\n";
				
				for (ParsedName property : comp.properites)
				{
					if (std::find(property.meta.begin(), property.meta.end(), "Editor") != property.meta.end())
					{
						EditorElement(cppSourcefile, comp.name.name + "_", property.nameSpaces[0] + property.dataType, property.name);
					}
				}

				cppSourcefile += "			ImGui::TreePop();\n		}\n";
				if ((compFullSpacename + comp.name.name) != "VE::Components::TransformComponent")
				{
					cppSourcefile += "		if (ImGui::BeginPopupContextItem(0, 1))\n		{\n";
					cppSourcefile += "			if(ImGui::MenuItem(\"Remove\"))\n			{\n";
					cppSourcefile += "				entity.remove<" + compFullSpacename + comp.name.name + ">(); open = false;\n";
					cppSourcefile += "			}\n";
					cppSourcefile += "			ImGui::EndPopup();\n		}\n";
				}
				cppSourcefile += "	}\n";
			}
		}

		cppSourcefile += "\n	ImGui::PopID();\n}\n";


		std::cout << cppSourcefile << std::endl;

		std::ofstream output(generatedFilepath / filename);
		output << cppSourcefile;
		output.close();
	}




	void HeaderTool::GenerateSerializationFile()
	{
		std::string filename = "ve_generated_serialization_file.cpp";

		std::string cppSourcefile= "#include \"ve_defines.h\"\n#include \"scenes/ve_scene.h\"\n";
		
		for (const HeaderFile& headerFile : headerFiles)
		{
			cppSourcefile += "#include \"" + headerFile.headerFilepath.generic_string() + "\"\n\n";
		}

		cppSourcefile += "using namespace VE;\n";
		cppSourcefile += "using namespace VE::Components;\n";
		cppSourcefile += "using namespace VE::_Components;\n\n\n";

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
				cppSourcefile += "	VE::Scene::GetSingleton()->GetFlecsWorld().component<" + compFullSpacename + comp.name.name + ">();\n";
					
			}
			
		if (type == Type::Engine)
		{
			cppSourcefile += "	EngineGeneratedSerialization();\n";
		}
		else if (type == Type::Project)
		{
			cppSourcefile += "	ProjectGeneratedSerialization();\n";
		}

			//registere systems brother
		for (const HeaderFile& headerFile : headerFiles)
			for (const System& system : headerFile.systems)
			{
				std::string systemFullSpacename = "";
				for (std::string sn : system.nameSpaces)
				{
					systemFullSpacename += sn + "::";
				}
				cppSourcefile += "	 VE::Scene::GetSingleton()->GetFlecsWorld().system<";
				for (const ParsedName& com : system.components)
				{
					std::string compSpaceName = "";
					for (std::string sn : com.nameSpaces)
					{
						compSpaceName += sn + "::";
					}
					cppSourcefile += compSpaceName + com.name + ",";
				}
				cppSourcefile.pop_back();
				cppSourcefile += ">()";
				if (system.meta.empty())
				{
					cppSourcefile += ".kind(flecs::OnUpdate)";
				}
				else 
				{
					if (std::find(system.meta.begin(), system.meta.end(), "OnStart") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::OnStart)";
					}
					else if(std::find(system.meta.begin(), system.meta.end(), "PreFrame") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::PreFrame)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "OnLoad") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::OnLoad)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "PostLoad") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::PostLoad)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "PreUpdate") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::PreUpdate)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "OnUpdate") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::OnUpdate)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "OnValidate") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::OnValidate)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "PostUpdate") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::PostUpdate)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "PreStore") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::PreStore)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "OnStore") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::OnStore)";
					}
					else if (std::find(system.meta.begin(), system.meta.end(), "PostFrame") != system.meta.end())
					{
						cppSourcefile += ".kind(flecs::PostFrame)";
					}
					if (std::find(system.meta.begin(), system.meta.end(), "Multithreaded") != system.meta.end())
					{
						cppSourcefile += ".multi_threaded()";
					}
					//
				}
				cppSourcefile += ".each(" + systemFullSpacename + system.name + ");\n";
			}
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