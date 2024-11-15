#include "ve_editor.h"
#include "ve_editor_input.h"
#include "ve_engine.h"
#include <rlImGui.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include "entities/ve_entity.h"
#include "platform/ve_platform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <raymath.h>
#include "entities/ve_camera_2d_entity.h"
#include "scenes/ve_scene_2d.h"
#include "editor/ve_editor_elements.h"
namespace VE 
{
	Editor* Editor::singleton = nullptr;
	Editor::Editor(class Engine* engine) : engine(engine)
	{
		singleton = this;
		rlImGuiSetup(true);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	Editor::~Editor() 
	{
		rlImGuiShutdown();
	}
	Editor* Editor::GetSingleton()
	{
		return singleton;
	}
	void Editor::DrawUI()
	{
		// Start the Dear ImGui frame
		rlImGuiBegin();

		ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

		//ImGui::ShowDemoWindow();

		DrawMainMenuBar();

		DrawHierarchy();

		DrawInspector();

		DrawStatus();

		DrawSceneViewport();

		DrawGameViewport();

		rlImGuiEnd();
	}

	void Editor::DrawChildren(Entity* entity, std::list<Entity*>& deletedEntities)
	{
		for (auto* child : entity->children) 
		{
			ImGuiTreeNodeFlags flags = ((engine->sceneManager->selectedEntity == (child)) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool open = ImGui::TreeNodeEx((void*)((uint64_t)(child)), flags, child->name.c_str());
			
			if (ImGui::IsItemClicked())
			{
				engine->sceneManager->selectedEntity = child;
			}

			if (ImGui::BeginPopupContextItem(0, 1))
			{
				engine->sceneManager->selectedEntity = child;
				if (ImGui::BeginMenu("Add Child"))
				{
					for (std::string entityName : engine->entitiesRegistry)
					{
						if (ImGui::MenuItem(entityName.c_str()))
						{
							Entity* childEntity = CreateProjectEntity(entityName);
							if (!childEntity)
							{
								childEntity = engine->CreateBuiltinEntity(entityName);
							}
							engine->sceneManager->selectedEntity->AddChild(childEntity);
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Remove"))
				{
					if (child == engine->sceneManager->selectedEntity)
					{
						engine->sceneManager->selectedEntity = nullptr;
					}
					deletedEntities.push_back(child);
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				if (child->children.size() > 0)
				{
					DrawChildren(child, deletedEntities);
				}
				ImGui::TreePop();
			}
		}
	}

	void Editor::AddEntityNode(Entity* entity, std::list<Entity*>& deletedEntities)
	{
		if (entity->parent)
		{
			return;
		}
		ImGuiTreeNodeFlags flags = ((engine->sceneManager->selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)((uint64_t)entity), flags, entity->name.c_str());
		if (ImGui::IsItemClicked())
		{
			engine->sceneManager->selectedEntity = entity;
		}

		if (ImGui::BeginPopupContextItem(0, 1))
		{
			engine->sceneManager->selectedEntity = entity;
			if (ImGui::BeginMenu("Add Child"))
			{
				for (std::string entityName : engine->entitiesRegistry)
				{
					if (ImGui::MenuItem(entityName.c_str()))
					{
						Entity* childEntity = CreateProjectEntity(entityName);
						if (!childEntity)
						{
							childEntity = engine->CreateBuiltinEntity(entityName);
						}
						engine->sceneManager->selectedEntity->AddChild(childEntity);
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Remove"))
			{
				if (entity == engine->sceneManager->selectedEntity)
				{
					engine->sceneManager->selectedEntity = nullptr;
				}
				deletedEntities.push_back(entity);
			}
			ImGui::EndPopup();
		}
		if (opened)
		{
			if (entity->children.size() > 0)
			{
				//if node is expaneded, draw children.
				DrawChildren(entity, deletedEntities);
			}
			ImGui::TreePop();
		}

	}
	void Editor::DrawHierarchy()
	{
		ImGui::Begin("Hierarchy");
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_None))
		{
			engine->sceneManager->selectedEntity = nullptr;
		}
		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::BeginMenu("Add Entity"))
			{
				for (std::string entityName : engine->entitiesRegistry)
				{
					if (ImGui::MenuItem(entityName.c_str()))
					{
						Entity* createdEntity = engine->CreateBuiltinEntity(entityName);
						if (createdEntity == nullptr)
							createdEntity = CreateProjectEntity(entityName);
						if (createdEntity == nullptr)
						{
							TraceLog(LOG_ERROR, "Failed to create entity (%s)!", entityName.c_str());
							continue;
						}
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Add Construct"))
			{
				std::filesystem::path constructPath = VE::OpenFileDialog();
				Engine::GetSingleton()->GetSceneManager()->LoadScene(constructPath);
				//add construct
			}
			ImGui::EndPopup();
		}
		size_t index = 0;
		std::list<Entity*> deletedEntities;
		for (auto itr = engine->sceneManager->currentScene->entities.begin(); itr != engine->sceneManager->currentScene->entities.end(); itr++)
		{
			AddEntityNode(*itr, deletedEntities);
			if (itr == engine->sceneManager->currentScene->entities.end() || engine->sceneManager->currentScene->entities.size() == 0)
			{
				break;
			}
		}

		for (auto ent : deletedEntities)
		{
			engine->sceneManager->currentScene->DeleteEntityChildren(ent);
			engine->sceneManager->currentScene->entities.remove(ent);
			if (ent->parent)
			{
				ent->parent->children.remove(ent);
			}
			delete ent;
		}

		ImGui::End();
	}

	void Editor::DrawMainMenuBar()
	{
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Scene"))
			{
				std::filesystem::path scenePath = OpenFileDialog();
				if (!scenePath.empty())
				{
					engine->sceneManager->LoadScene(scenePath);
				}
			}
			if (ImGui::BeginMenu("New Scene"))
			{
				if (ImGui::MenuItem("2D scene"))
				{
					std::filesystem::path scenePath = VE::SaveFileDialog();
					if (!scenePath.empty())
					{
						engine->sceneManager->UnloadScene();
						//TODO: show a dialog to choose scene type 2d or 3d.
						engine->sceneManager->currentScene = new Scene2D();
						engine->sceneManager->currentScene->sceneType = SceneType::Scene2D;
						engine->sceneManager->currentScene->scenePath = scenePath;
						engine->sceneManager->currentScene->name = scenePath.filename().string();
						//Add default entites like camera and basic lighting.
						engine->sceneManager->SaveScene();
					}
					
				}
				if (ImGui::MenuItem("3D scene"))
				{
					//handle 3d scene creation.
				}
				ImGui::EndMenu();
				
			}
			if (ImGui::MenuItem("Save Scene"))
			{
				engine->sceneManager->SaveScene();
			}
			if (ImGui::MenuItem("Save Scene As"))
			{
				engine->sceneManager->SaveSceneAs();
			}

			ImGui::EndMenu();
		}

		float menuBarWidth = ImGui::GetWindowWidth();
		float buttonWidth = ImGui::CalcTextSize("Start").x + ImGui::CalcTextSize("Reload Project").x + ImGui::GetStyle().ItemSpacing.x * 4;
		float spacing = (menuBarWidth - buttonWidth) / 2.0f;
		ImGui::SameLine(spacing);
		if (engine->sceneManager->mode == SceneMode::Editor)
		{
			if (ImGui::Button("Start"))
			{
				engine->sceneManager->SaveScene();
				if (!engine->sceneManager->currentScene->scenePath.empty())
				{
					engine->sceneManager->mode = SceneMode::Game;
					std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
					SceneType sceneType = engine->sceneManager->currentScene->GetSceneType();
					Camera2D editorCameraSate;
					if (sceneType == SceneType::Scene2D)
					{
						editorCameraSate = ((Scene2D*)(engine->sceneManager->currentScene))->editorCamera;
					}
					else
					{
						//handle 3d scene
					}
					engine->sceneManager->UnloadScene();
					engine->ReloadProjectSharedLibrary();
					engine->sceneManager->LoadScene(reloadScenePath);
					if (sceneType == SceneType::Scene2D)
					{
						((Scene2D*)(engine->sceneManager->currentScene))->editorCamera = editorCameraSate;
					}
					else
					{
						//handle 3d scene
					}
					ImGui::SetWindowFocus("GameViewport");
				}
				
			}

			ImGui::SameLine();

			if (ImGui::Button("Reload Project"))
			{
				engine->sceneManager->SaveScene();
				std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
				SceneType sceneType = engine->sceneManager->currentScene->GetSceneType();
				Camera2D editorCameraSate;
				if (sceneType == SceneType::Scene2D)
				{
					editorCameraSate = ((Scene2D*)(engine->sceneManager->currentScene))->editorCamera;
				}
				else
				{
					//handle 3d scene
				}
				engine->sceneManager->UnloadScene();
				engine->ReloadProjectSharedLibrary();
				engine->sceneManager->LoadScene(reloadScenePath);
				if (sceneType == SceneType::Scene2D)
				{
					((Scene2D*)(engine->sceneManager->currentScene))->editorCamera = editorCameraSate;
				}
				else
				{
					//handle 3d scene
				}
			}
		}
		else
		{
			if (ImGui::Button("End"))
			{
				engine->sceneManager->mode = SceneMode::Editor;
				std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
				SceneType sceneType = engine->sceneManager->currentScene->GetSceneType();
				Camera2D editorCameraSate;
				if (sceneType == SceneType::Scene2D)
				{
					editorCameraSate = ((Scene2D*)(engine->sceneManager->currentScene))->editorCamera;
				}
				else
				{
					//handle 3d scene
				}
				engine->sceneManager->UnloadScene();
				engine->sceneManager->LoadScene(reloadScenePath);
				if (sceneType == SceneType::Scene2D)
				{
					((Scene2D*)(engine->sceneManager->currentScene))->editorCamera = editorCameraSate;
				}
				else
				{
					//handle 3d scene
				}
				ImGui::SetWindowFocus("SceneViewport");
			}
		}
		ImGui::EndMainMenuBar();
	}
	void Editor::DrawInspector()
	{
		ImGui::Begin("Inspector");
		if (engine->sceneManager->selectedEntity)
		{
			engine->sceneManager->selectedEntity->ComponentDrawEditorUI();
			engine->sceneManager->selectedEntity->DrawEditorUI();
		}
		else
		{
			//Scene Settings
			if (engine->sceneManager->currentScene->sceneType == SceneType::Scene2D)
			{
				Scene2D* s2d = (Scene2D*)engine->sceneManager->currentScene;
				EditorElement::Color(s2d->clearColor, "Clear Color");
			}
		}
		ImGui::End();
	}

	void Editor::DrawStatus()
	{
		ImGui::Begin("Status");
		float frameTime = GetFrameTime();
		float fps = 1.0f / frameTime;
		ImGui::Text("Frame Time: %fms", frameTime);
		ImGui::Text("FPS       : %f", fps);
		ImGui::End();
	}

	void Editor::DrawSceneViewport()
	{
		ImGui::Begin("SceneViewport");
		sceneViewportFocused = ImGui::IsWindowFocused() ? true : false;
		ImVec2 size = ImGui::GetContentRegionAvail();
		sceneViewportSize = *((glm::vec2*)&size);
		size = ImGui::GetCursorScreenPos();
		sceneViewportPosition = *((glm::vec2*)&(size));


		const RenderTexture* sceneViewTex = &engine->sceneManager->currentScene->editorCameraRenderTarget;
		rlImGuiImageRenderTextureFit(&sceneViewTex->texture, false);

		//Draw ImGuizmo stuff here.
		if (SceneType::Scene2D == engine->GetSceneManager()->currentScene->sceneType)
		{
			Scene2D* s2d = (Scene2D*)engine->GetSceneManager()->currentScene;
			ImGuizmo::BeginFrame();
			Entity* selectedEntity = engine->sceneManager->selectedEntity;
			if (selectedEntity)
			{
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(sceneViewportPosition.x, sceneViewportPosition.y, sceneViewportSize.x, sceneViewportSize.y);
				glm::mat4 projectionMatrix = glm::ortho(0.0f, sceneViewportSize.x, sceneViewportSize.y, 0.0f);

				glm::mat4 transformMatrix = selectedEntity->transformComponent->GetWorldTransformMatrix();
				Matrix camreaViewMatrix = GetCameraMatrix2D(s2d->editorCamera);
				ImGuizmo::Manipulate(MatrixToFloat(camreaViewMatrix), glm::value_ptr(projectionMatrix), ImGuizmo::TRANSLATE | ImGuizmo::SCALE | ImGuizmo::ROTATE, ImGuizmo::WORLD, glm::value_ptr(transformMatrix));

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 skew;
					glm::vec3 pos;
					glm::vec3 scl;
					glm::vec4 pres;
					glm::quat rot;
					if (selectedEntity->GetParent())
					{
						glm::decompose(transformMatrix, scl, rot, pos, skew, pres);

						selectedEntity->transformComponent->position += pos - selectedEntity->transformComponent->worldPosition;
						selectedEntity->transformComponent->scale += scl - selectedEntity->transformComponent->worldScale;
						glm::vec3 eulerAngles = glm::eulerAngles(rot);
						eulerAngles = glm::degrees(eulerAngles);
						selectedEntity->transformComponent->rotation += eulerAngles - selectedEntity->transformComponent->worldRotation;
					}
					else 
					{
						glm::decompose(transformMatrix, selectedEntity->transformComponent->scale, rot, selectedEntity->transformComponent->position, skew, pres);
						glm::vec3 eulerAngles = glm::eulerAngles(rot);
						eulerAngles = glm::degrees(eulerAngles);
						selectedEntity->transformComponent->rotation = eulerAngles;
					}

					
				}

			}
		}
		else 
		{
			//handle 3d scene here
		}
		

		ImGui::End();
	}

	void Editor::DrawGameViewport()
	{
		ImGui::Begin("GameViewport");
		ImVec2 size = ImGui::GetWindowSize();
		gameViewportSize = *((glm::vec2*)&size);
		size = ImGui::GetWindowPos();
		gameViewportPosition = *((glm::vec2*)&size);
		gameViewportFocused = ImGui::IsWindowFocused() ? true : false;
		if (SceneType::Scene2D == engine->sceneManager->currentScene->sceneType)
		{
			Scene2D* s2d = (Scene2D*)engine->sceneManager->currentScene;
			if (s2d->mainCamera)
			{
				const RenderTexture2D* gameViewTex = s2d->mainCamera->GetRenderTarget();

				rlImGuiImageRenderTextureFit(&gameViewTex->texture, true);
			}
		}
		else 
		{
			//handle 3d scene here
		}
		ImGui::End();
	}

	void Editor::UpdateEditor(float deltaTime)
	{
		if (sceneViewportFocused)
		{
			if (SceneType::Scene2D == engine->sceneManager->currentScene->sceneType)
			{
				Scene2D* s2d = (Scene2D*)engine->sceneManager->currentScene;
				
				Vector2 mouseWorldPos = GetScreenToWorld2D(Vector2{ EditorInput::GetMousePostion().x, EditorInput::GetMousePostion().y },
					s2d->editorCamera);

				if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
				{
					Vector2 delta = GetMouseDelta();
					delta.x *= -1.0f / s2d->editorCamera.zoom;
					delta.y *= -1.0f / s2d->editorCamera.zoom;
					s2d->editorCamera.target.x = s2d->editorCamera.target.x + delta.x;
					s2d->editorCamera.target.y = s2d->editorCamera.target.y + delta.y;
				}


				float wheel = GetMouseWheelMove();
				if (wheel)
				{

					s2d->editorCamera.offset = Vector2{ EditorInput::GetMousePostion().x, EditorInput::GetMousePostion().y };
					s2d->editorCamera.target = mouseWorldPos;
					float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
					if (wheel < 0) scaleFactor = 1.0f / scaleFactor;
					s2d->editorCamera.zoom = glm::clamp(s2d->editorCamera.zoom * scaleFactor, 0.005f, 64.0f);
				}
			}
			
		}
	}



}
