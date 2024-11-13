#include "project_dialog.h"
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include "platform/ve_platform.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include "utils/ve_serialization.h"
#include <glm/glm.hpp>
#include <fstream>
#include <cstdlib> 
ProjectDialog::ProjectDialog()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(512, 512, "Project Dialog");

	rlImGuiSetup(true);
}

ProjectDialog::~ProjectDialog()
{
	rlImGuiShutdown();
}

void copy_directory(const std::filesystem::path& source, const std::filesystem::path& destination)
{
	std::filesystem::create_directory(destination);  // Create the destination directory

	for (const auto& entry : std::filesystem::directory_iterator(source)) {
		const auto& path = entry.path();
		auto dest_path = destination / path.filename();
		if (std::filesystem::is_directory(path)) {
			copy_directory(path, dest_path);  // Recursive call for directories
		}
		else {
			std::filesystem::copy(path, dest_path);  // Copy files
		}
	}
}

bool ProjectDialog::Run(VE::ProjectDetails* projectDetails)
{
	bool result = false;
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		rlImGuiBegin();
		
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(512, 512));
		ImGui::Begin("FullScreenWindow", nullptr, windowFlags);

		
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 windowCenter = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);

		ImGui::SetCursorPosX(windowCenter.x - 50);  
		ImGui::SetCursorPosY(windowCenter.y - 30);  

		
		if (ImGui::Button("Open Project", ImVec2(120, 30))) 
		{
			projectDetails->path = VE::OpenFileDialog();
			if (!projectDetails->path.empty())
			{
				result = true;
			}
		}

		
		ImGui::SetCursorPosX(windowCenter.x - 50);  
		ImGui::SetCursorPosY(windowCenter.y + 10);  

		if (ImGui::Button("Create Project", ImVec2(120, 30))) 
		{
			openProjectDetailsPopup = true;
		}

		if (openProjectDetailsPopup)
		{
			ImGui::OpenPopup("Project Details");
			ImGui::SetNextWindowSize({ 512, 512 });
			ImGui::SetNextWindowPos({ 0, 0 });
		}
		ImGuiWindowFlags projectDetailsPopupFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
		if (ImGui::BeginPopupModal("Project Details", 0, projectDetailsPopupFlags))
		{
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 windowCenter = ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f);

			float verticalOffset = 50.0f;
			ImGui::SetCursorPosY(windowCenter.y - verticalOffset);

			ImGui::SetCursorPosX((windowSize.x - 180) * 0.5f);
			if (ImGui::Button("Choose Project Directory", ImVec2(180, 30))) 
			{
				projectDetails->path = VE::SaveFileDialog(VE_PROJECT_FILE_EXTENSION);
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::SetCursorPosX((windowSize.x - 190) * 0.5f);
			ImGui::Text("Scene Type:");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			static int selectedSceneType = 0;
			const char* sceneTypes[] = { "3D", "2D" };
			if (ImGui::BeginCombo("##SceneType", sceneTypes[selectedSceneType])) 
			{
				for (int i = 0; i < IM_ARRAYSIZE(sceneTypes); i++) 
				{
					bool isSelected = (selectedSceneType == i);
					if (ImGui::Selectable(sceneTypes[i], isSelected)) 
					{
						selectedSceneType = i;
					}
					if (isSelected) 
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Dummy(ImVec2(0.0f, ImGui::GetContentRegionAvail().y - 50));
			ImGui::Separator();

			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 160);
			if (ImGui::Button("Cancel", ImVec2(70, 30))) 
			{
				ImGui::CloseCurrentPopup();
				openProjectDetailsPopup = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Create", ImVec2(70, 30))) 
			{
				if (!projectDetails->path.empty())
				{
					if ((std::string)sceneTypes[selectedSceneType] == "2D")
					{
						//create project
						nlohmann::json projectJson;
						projectJson["project_name"] = projectDetails->path.stem();
						projectJson["title"] = projectDetails->path.stem();
						projectJson["width"] = 1920;
						projectJson["height"] = 1080;
						projectJson["render_width"] = 1920;
						projectJson["render_height"] = 1080;
						projectJson["vsync"] = true;
						projectJson["main_scene_path"] = "";

						if (projectDetails->path.extension().string() != ("." + (std::string)VE_PROJECT_FILE_EXTENSION))
						{
							TraceLog(LOG_ERROR, "Project files must have %s extension!", VE_PROJECT_FILE_EXTENSION);
							
						}
						else 
						{
							if (!std::filesystem::exists(projectDetails->path.parent_path().string() + "/" + projectDetails->path.stem().string()))
							{
								std::filesystem::create_directories(projectDetails->path.parent_path().string() + "/" + projectDetails->path.stem().string());

								copy_directory("template_project", projectDetails->path.parent_path().string() + "/" + projectDetails->path.stem().string());

								projectDetails->path = (projectDetails->path.parent_path().string() + "/" + projectDetails->path.stem().string() + "/" + projectDetails->path.stem().string() + "." + VE_PROJECT_FILE_EXTENSION);
							}
							std::string changeDirectoryCommand = "cd " + projectDetails->path.parent_path().string() + "/" + "&& build_project.bat Debug";

							// Run the command
							system(changeDirectoryCommand.c_str());
							std::ofstream projectFile(projectDetails->path);
							projectFile << projectJson;
							projectFile.close();

							result = true;
						}
					}
					else if((std::string)sceneTypes[selectedSceneType] == "3D")
					{
					
					}
				}
			}
			ImGui::EndPopup();
		}


		ImGui::End();

		rlImGuiEnd();
		EndDrawing();

		if (result)
		{
			CloseWindow();
		}
	}

	return result;
}
