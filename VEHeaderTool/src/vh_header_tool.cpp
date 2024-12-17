#include "vh_header_tool.h"
#include <fstream>
#include <iostream>
#include <random>
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

	void HeaderTool::EditorElement(std::string& cppSourefile, std::string compPTR, std::string dataType, std::string name, std::vector<Meta>& propertyMeta, bool isVector)
	{
		std::string formatedName = FormatName(name);
		if (dataType.find("vec2") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Vec2(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("vec3") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Vec3(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("vec4") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Vec4(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("NormalizedColor") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Color(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		
		else if (dataType.find("float") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Float(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("double") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Double(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("int") != std::string::npos || dataType.find("size_t") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Int(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("string") != std::string::npos)
		{
			cppSourefile += "			EditorElement::String(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("path") != std::string::npos)
		{
			cppSourefile += "			EditorElement::FileSystem(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";
		}
		else if (dataType.find("bool") != std::string::npos)
		{
			cppSourefile += "			EditorElement::Checkbox(" + compPTR + "->" + name + (isVector ? "[i]" : "") + ", \"" + formatedName + "\"";
			auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
			if (itr != propertyMeta.end())
			{
				cppSourefile += ", " + compPTR + ", ";
				Meta meta = *itr;
				Callback callBack;
				for (const HeaderFile& headerFile : headerFiles)
				{
					auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
					if (iterator != headerFile.callbacks.end())
					{
						callBack = *iterator;
						break;
					}

				}
				std::string fullNameSpace = "";
				for (std::string sn : callBack.nameSpaces)
				{
					fullNameSpace += sn + "::";
				}
				cppSourefile += fullNameSpace + callBack.name;
			}
			cppSourefile += ");\n";

		}
		else 
		{
			for (auto headerFile : headerFiles)
			{
				for (auto _enum : headerFile.enums)
				{
					if (dataType.find(_enum.name) != std::string::npos)
					{
						//VE_API void Enum(const char** items, int* currentItem, size_t items_count, std::string label);

						cppSourefile += "			const char* items[] = { \n";
						for (auto item : _enum.items)
						{
							cppSourefile += "					\"" + item + "\",\n";
						}
						cppSourefile += "			};\n";
						cppSourefile += "			EditorElement::Combo(items, (int*)&(" + compPTR + "->" + name + (isVector? "[i]" : "") +  "), " + std::to_string(_enum.items.size()) + ", \"" + formatedName + "\"";

						auto itr = std::find_if(propertyMeta.begin(), propertyMeta.end(), [](const Meta& meta) { return meta.key == "OnChange"; });
						if (itr != propertyMeta.end())
						{
							cppSourefile += ", " + compPTR + ", ";
							Meta meta = *itr;
							Callback callBack;
							for (const HeaderFile& headerFile : headerFiles)
							{
								auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
								if (iterator != headerFile.callbacks.end())
								{
									callBack = *iterator;
									break;
								}

							}
							std::string fullNameSpace = "";
							for (std::string sn : callBack.nameSpaces)
							{
								fullNameSpace += sn + "::";
							}
							cppSourefile += fullNameSpace + callBack.name;
						}
						cppSourefile += ");\n";

						break;
					}
				}
			}

			for (auto headerFile : headerFiles)
			{
				for (auto comp : headerFile.components)
				{
					if (dataType.find(comp.name.name) != std::string::npos)
					{
						std::string compFullSpacename = "";
						for (std::string sn : comp.name.nameSpaces)
						{
							compFullSpacename += sn + "::";
						}

						cppSourefile += "			" + compFullSpacename + comp.name.name + "* " + comp.name.name + "_ = &" + (isVector? compPTR + "->" + name + "[i];\n" : compPTR + "->" + name + ";\n");
						cppSourefile += "			Draw" + comp.name.name + "EditorUI(\"" + name + "\", entity, " + comp.name.name + "_" + ", false); \n";
					}
				}
			}
		}

		cppSourefile += "\n\n";

	}
	void HeaderTool::GenerateEditorUIFile()
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> random(0, 1000);


		std::string filename = "ve_generated_editor_file.cpp";

		std::string cppSourcefile = "#ifdef VE_EDITOR\n";


		cppSourcefile += "#include \"ve_defines.h\"\n#include \"scenes/ve_scene.h\"\n#include \"editor/ve_editor.h\"\n#include <imgui.h>\n#include \"editor/ve_editor_elements.h\"\n#include \"components/ve_components.h\"\n\n";

		for (const HeaderFile& headerFile : headerFiles)
		{
			cppSourcefile += "#include \"" + headerFile.headerFilepath.generic_string() + "\"\n\n";
		}

		cppSourcefile += "using namespace VE;\n";
		cppSourcefile += "using namespace VE::Components;\n";
		cppSourcefile += "using namespace VE::_Components;\n";
		cppSourcefile += "using namespace VE::Components::UI;\n\n\n";

		for (const HeaderFile& headerFile : headerFiles)
		{
			for (const Component& comp : headerFile.components)
			{
				std::string compFullSpacename = "";
				for (std::string sn : comp.name.nameSpaces)
				{
					compFullSpacename += sn + "::";
				}
				cppSourcefile += "void Draw" + comp.name.name + "EditorUI(std::string name, flecs::entity entity, " + compFullSpacename + comp.name.name + "* " + comp.name.name + "_, bool removable)\n{\n";

				cppSourcefile += "	ImGui::PushID((name + (const char*)(int[1])(" + std::to_string(random(rng)) + ")).c_str()); \n"
					"	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;\n";
				cppSourcefile += "		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());\n";
				cppSourcefile += "		if(removable)\n";
				cppSourcefile += "		if (ImGui::BeginPopupContextItem(0, 1))\n		{\n";
				cppSourcefile += "			if(ImGui::MenuItem(\"Remove\"))\n			{\n";
				cppSourcefile += "				entity.remove<" + compFullSpacename + comp.name.name + ">();\n";
				cppSourcefile += "			}\n";
				cppSourcefile += "			ImGui::EndPopup();\n		}\n";
				
				cppSourcefile += "		if (open)\n		{\n"
					"			\n";
				
				
				for (ParsedName property : comp.properites)
				{
					if (std::find_if(property.meta.begin(), property.meta.end(), [](const Meta& meta) { return meta.key == "Editor";}) != property.meta.end())
					{
						if (property.dataType == "vector")
						{
							cppSourcefile += "\t\t\t{ImGui::PushID(((std::string)\"" + property.name + "\" + (const char*)(int[1])(" + std::to_string(random(rng)) + ")).c_str()); \n";
							cppSourcefile += "\t\t\tImGuiTreeNodeFlags _flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;\n";
							cppSourcefile += "\t\t\tbool _open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), _flags, \"" + property.name + "\");\n";
							cppSourcefile += "\t\t\tImGui::SameLine();\n"
								"\t\t\tImVec2 plusSize = ImGui::CalcTextSize(\"+\");\n"
								"\t\t\tImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));\n"
								"\t\t\tImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));\n"
								"\t\t\tif(ImGui::Button(\"+\", {plusSize.x * 4.0f, plusSize.y * 0.85f})){" + comp.name.name + "_" + "->" + property.name + ".push_back({});}\n"
								"\t\t\tImGui::PopStyleVar(2);\n"
								"\t\t\tint elementIndexToRemove = -1;\n";
							cppSourcefile += "\t\t\tif(_open){\n";
							cppSourcefile += "\n\t\t\t\tfor(size_t i = 0; i < " + comp.name.name + "_" + "->" + property.name + ".size(); i++){\n"
							"\t\t\t\t\t\tImGui::PushID(((std::string)\"" + property.name + "\" + (const char*)(size_t[1])i).c_str());\n";
							cppSourcefile += "\t\t\t\t\t\tImGuiTreeNodeFlags __flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;\n";
							cppSourcefile += "\t\t\t\t\t\tbool __open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), __flags, ((std::string)\"" + property.name + "[\" + std::to_string(i) + \"]\").c_str());\n";
							cppSourcefile += "\t\t\t\t\t\tImGui::SameLine();\n"
								"\t\t\t\t\tImVec2 plusSize = ImGui::CalcTextSize(\"-\");\n"
								"\t\t\t\t\tImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));\n"
								"\t\t\t\t\tImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));\n"
								"\t\t\t\t\tif(ImGui::Button(\"-\", {plusSize.x * 4.0f, plusSize.y})){elementIndexToRemove = (int)i;}\n"
								"\t\t\t\t\tImGui::PopStyleVar(2);\n";
							cppSourcefile += "\t\t\t\t\t\tif(__open){\n";
							EditorElement(cppSourcefile, comp.name.name + "_", property.nameSpaces[0] + property.args[0], property.name, property.meta, true);
							cppSourcefile += "\t\t\t\t\t\tImGui::TreePop();}\n";
							cppSourcefile += "\t\t\t\t\t\tImGui::PopID();\n";
							cppSourcefile += "\t\t\t\t\t}\n";
							cppSourcefile += "\t\t\tif(elementIndexToRemove > 0 && elementIndexToRemove < " + comp.name.name + "_" + "->" + property.name + ".size()) " + comp.name.name + "_" + "->" + property.name + ".erase(" + comp.name.name + "_" + "->" + property.name + ".begin() + elementIndexToRemove);";
							cppSourcefile += "\n\t\t\tImGui::TreePop();\n}\n";
							cppSourcefile += "\t\t\tImGui::PopID();}\n";
						}
						else 
						{
							EditorElement(cppSourcefile, comp.name.name + "_", property.nameSpaces[0] + property.dataType, property.name, property.meta);
						}
					}

				}

				cppSourcefile += "			ImGui::TreePop();\n		}\n";
				cppSourcefile += "	\tImGui::PopID();\n}\n";
			}
		}
		if (type == Type::Engine)
		{
			cppSourcefile += "void VE::Editor::DrawComponentElements(std::string name, flecs::entity entity)\n{\n";
		}
		else
		{
			cppSourcefile += "extern \"C\" VE_PROJECT_API void ProjectDrawComponentElements(std::string name, flecs::entity entity)\n{\n";

		}

		//call drawFunction
		for (const HeaderFile& headerFile : headerFiles)
		{
			for (const Component& comp : headerFile.components)
			{
				std::string compFullSpacename = "";
				for (std::string sn : comp.name.nameSpaces)
				{
					compFullSpacename += sn + "::";
				}

				cppSourcefile += "	if(name == \"" + comp.name.name + "\"){\n";
			    cppSourcefile += "			" + compFullSpacename + comp.name.name + "* " + comp.name.name + "_ = entity.get_mut<" + compFullSpacename + comp.name.name + ">();\n";
				cppSourcefile += "		Draw" + comp.name.name + "EditorUI(name, entity," + comp.name.name + "_,";
				if (comp.name.name == "TransformComponent")
				{
					cppSourcefile += " false";
				}
				else 
				{
					cppSourcefile += " true";
				}
				cppSourcefile += ");\n	}\n";
			}
		}
		cppSourcefile += "\n}\n";

		cppSourcefile += "#endif\n";


		std::cout << cppSourcefile << std::endl;

		std::ofstream output(generatedFilepath / filename);
		output << cppSourcefile;
		output.close();
	}




	void HeaderTool::GenerateSerializationFile()
	{
		std::string filename = "ve_generated_serialization_file.cpp";

		std::string cppSourcefile= "#include \"ve_defines.h\"\n#include \"scenes/ve_scene.h\"\n#include \"components/ve_components.h\"\n\n";
		
		for (const HeaderFile& headerFile : headerFiles)
		{
			cppSourcefile += "#include \"" + headerFile.headerFilepath.generic_string() + "\"\n\n";
		}

		cppSourcefile += "using namespace VE;\n";
		cppSourcefile += "using namespace VE::Components;\n";
		cppSourcefile += "using namespace VE::Components::UI;\n";
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
						cppSourcefile += property.dataType;
						if (property.templateType)
						{
							cppSourcefile += "<";
							for (auto arg : property.args)
							{
								cppSourcefile += arg + ",";
							}
							if (property.args.size())
							{
								cppSourcefile.pop_back();
							}
							cppSourcefile += ">";
						}
						cppSourcefile += ">(\"" + property.name + "\")";
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
						"		});\n";
					cppSourcefile += "		flecs::world w = comp->world();\n"
						"		w.component<std::vector<" + compFullSpacename + comp.name.name + ">>().opaque(VE::std_vector_support<" + compFullSpacename + comp.name.name + ">);\n"
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
				cppSourcefile += "	VE::Scene::GetSingleton()->GetFlecsWorld().component<" + compFullSpacename + comp.name.name + ">()";
				
				auto itr = std::find_if(comp.name.meta.begin(), comp.name.meta.end(), [](const Meta& meta) { return meta.key == "OnAdd"; });

				if (itr != comp.name.meta.end())
				{
					Meta meta = *itr;
					Callback callBack;
					for (const HeaderFile& headerFile : headerFiles)
					{
						auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
						if (iterator != headerFile.callbacks.end())
						{
							callBack = *iterator;
							break;
						}
	
					}

					std::string fullNameSpace = "";
					for (std::string sn : callBack.nameSpaces)
					{
						fullNameSpace += sn + "::";
					}
					cppSourcefile += ".on_add(" + fullNameSpace + callBack.name + ")";
				}


				itr = std::find_if(comp.name.meta.begin(), comp.name.meta.end(), [](const Meta& meta) { return meta.key == "OnRemove"; });

				if (itr != comp.name.meta.end())
				{
					Meta meta = *itr;
					Callback callBack;
					for (const HeaderFile& headerFile : headerFiles)
					{
						auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
						if (iterator != headerFile.callbacks.end())
						{
							callBack = *iterator;
							break;
						}

					}
					std::string fullNameSpace = "";
					for (std::string sn : callBack.nameSpaces)
					{
						fullNameSpace += sn + "::";
					}
					cppSourcefile += ".on_remove(" + fullNameSpace + callBack.name + ")";
				}


				itr = std::find_if(comp.name.meta.begin(), comp.name.meta.end(), [](const Meta& meta) { return meta.key == "OnSet"; });

				if (itr != comp.name.meta.end())
				{
					Meta meta = *itr;
					Callback callBack;
					for (const HeaderFile& headerFile : headerFiles)
					{
						auto iterator = std::find_if(headerFile.callbacks.begin(), headerFile.callbacks.end(), [&](const Callback& _cb) { return _cb.name == meta.value; });
						if (iterator != headerFile.callbacks.end())
						{
							callBack = *iterator;
							break;
						}

					}
					std::string fullNameSpace = "";
					for (std::string sn : callBack.nameSpaces)
					{
						fullNameSpace += sn + "::";
					}
					cppSourcefile += ".on_set(" + fullNameSpace + callBack.name + ")";
				}

				cppSourcefile += ";\n";
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
				cppSourcefile += "	VE::Scene::GetSingleton()->GetFlecsWorld().system<";
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
				cppSourcefile += ">(\"" + system.name + "\")";
				
				if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "OnStart";}) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::OnStart)";
				}
				else if(std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "PreFrame"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::PreFrame)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "OnLoad"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::OnLoad)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "PostLoad"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::PostLoad)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "PreUpdate"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::PreUpdate)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "OnUpdate"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::OnUpdate)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "OnValidate"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::OnValidate)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "PostUpdate"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::PostUpdate)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "PreStore"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::PreStore)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "OnStore"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::OnStore)";
				}
				else if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "PostFrame"; }) != system.meta.end())
				{
					cppSourcefile += ".kind(flecs::PostFrame)";
				}
				else 
				{
					cppSourcefile += ".kind(flecs::OnUpdate)";
				}


				if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "Multithreaded"; }) != system.meta.end())
				{
					cppSourcefile += ".multi_threaded()";
				}
				if (std::find_if(system.meta.begin(), system.meta.end(), [](const Meta& meta) { return meta.key == "Immediate"; }) != system.meta.end())
				{
					cppSourcefile += ".immediate()";
				}
				
				cppSourcefile += ".without<_Components::Disabled>().each(" + systemFullSpacename + system.name + ");\n";
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