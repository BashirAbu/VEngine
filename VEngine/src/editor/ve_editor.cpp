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
#include "editor/ve_editor_elements.h"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_impl_raylib.h"

namespace VE 
{

	void GrayTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// Background
		colors[ImGuiCol_WindowBg] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f); // Darker soft gray
		colors[ImGuiCol_ChildBg] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Darker child background
		colors[ImGuiCol_PopupBg] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f); // Darker popup background

		// Main Menu Bar
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Even darker menu bar

		// Borders
		colors[ImGuiCol_Border] = ImVec4(0.40f, 0.40f, 0.40f, 0.60f); // Much darker borders
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		// Headers
		colors[ImGuiCol_Header] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Darker header
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Darker active header

		// Buttons
		colors[ImGuiCol_Button] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Darker button background
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Hovered button
		colors[ImGuiCol_ButtonActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Active button

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Darker frame background
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Darker active frame

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Darker tab
		colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f); // Darker active tab
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Darker title bar, more visible
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.50f, 0.50f, 0.50f, 0.75f);

		// Scrollbar
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f); // Darker scrollbar background
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f); // Darker grab
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

		// Sliders
		colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f); // Darker slider
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // Darker active slider

		// Separator
		colors[ImGuiCol_Separator] = ImVec4(0.35f, 0.35f, 0.35f, 0.60f); // Darker separator
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

		// Text
		colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Darker text for better contrast
		colors[ImGuiCol_TextDisabled] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);

		// Adjust the style for a deeper, darker look
		style.WindowRounding = 4.0f;
		style.FrameRounding = 3.0f;
		style.GrabRounding = 3.0f;
		style.ScrollbarRounding = 3.0f;
		style.ChildRounding = 3.0f;
		style.FramePadding = ImVec2(6.0f, 4.0f); // More padding for a spacious feel
	}

	Editor* Editor::singleton = nullptr;
	Editor::Editor(class Engine* engine) : engine(engine)
	{
		singleton = this;
		rlImGuiSetup(true);

		ImGuiIO& io = (ImGui::GetIO());
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable docking
		
		GrayTheme();

		font = io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans-Medium.ttf", 18.0f);
		IM_ASSERT(font != nullptr);

		// required to be called to cache the font texture with raylib
		io.Fonts->Build();
		ImGui_ImplRaylib_BuildFontAtlas();


		colorPickingShader = LoadShader(0, "resources/shaders/color_picking_shader.fs");
		editorCamera = {};
		editorCamera.zoom = 1.0f;
	}
	Editor::~Editor() 
	{
		UnloadShader(colorPickingShader);
		UnloadRenderTexture(editorCameraRenderTarget);
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
		ImGui::PushFont(font);
		ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::ShowDemoWindow();

		DrawMainMenuBar();

		DrawHierarchy();

		DrawInspector();

		DrawStatus();

		DrawSceneViewport();

		DrawGameViewport();
		ImGui::PopFont();
		rlImGuiEnd();
		//make sure this gets called after DrawSceneViewport().
		UpdateEditor(GetFrameTime());
		
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
				Engine::GetSingleton()->GetSceneManager()->LoadConstruct(constructPath);
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
						engine->sceneManager->currentScene = new Scene(SceneType::Scene2D);
						engine->sceneManager->currentScene->scenePath = scenePath;
						engine->sceneManager->currentScene->name = scenePath.stem().string();
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
					
					engine->sceneManager->UnloadScene();
					engine->ReloadProjectSharedLibrary();
					engine->sceneManager->LoadScene(reloadScenePath);
					
					ImGui::SetWindowFocus("GameViewport");
				}
				
			}

			ImGui::SameLine();

			if (ImGui::Button("Reload Project"))
			{
				engine->sceneManager->SaveScene();
				std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
				
				engine->sceneManager->UnloadScene();
				engine->ReloadProjectSharedLibrary();
				engine->sceneManager->LoadScene(reloadScenePath);
				
			}
		}
		else
		{
			if (ImGui::Button("End"))
			{
				engine->sceneManager->mode = SceneMode::Editor;
				std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
				SceneType sceneType = engine->sceneManager->currentScene->GetSceneType();
				
				engine->sceneManager->UnloadScene();
				engine->sceneManager->LoadScene(reloadScenePath);
				
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
			char buffer[255];
			if (engine->sceneManager->selectedEntity->name.size() > 255)
			{
				engine->sceneManager->selectedEntity->name.resize(255);
			}
			strcpy(buffer, engine->sceneManager->selectedEntity->name.c_str());
			ImGui::InputText("Name", buffer, 255);
			engine->sceneManager->selectedEntity->name = buffer;


			engine->sceneManager->selectedEntity->ComponentDrawEditorUI();
			engine->sceneManager->selectedEntity->DrawEditorUI();
		}
		else
		{
			//Scene Settings
			EditorElement::Color(engine->sceneManager->currentScene->clearColor, "Clear Color");
			
		}
		ImGui::End();
	}

	void Editor::DrawStatus()
	{
		ImGui::Begin("Status");
		float frameTime = GetFrameTime();
		float fps = 1.0f / frameTime;
		ImGui::Text("Frame Time  : %fms", frameTime);
		ImGui::Text("FPS         : %f", fps);
		ImGui::Text("Entity Count: %d", engine->sceneManager->currentScene->entities.size());
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


		RenderEditorSceneView();

		const RenderTexture* sceneViewTex = &editorCameraRenderTarget;
		rlImGuiImageRenderTextureFit(&sceneViewTex->texture, false);

		//Draw ImGuizmo stuff here.
		
		ImGuizmo::BeginFrame();
		Entity* selectedEntity = engine->sceneManager->selectedEntity;
		usingImGuizmo = false;
		if (selectedEntity)
		{
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(sceneViewportPosition.x, sceneViewportPosition.y, sceneViewportSize.x, sceneViewportSize.y);
			glm::mat4 projectionMatrix = glm::ortho(0.0f, sceneViewportSize.x, sceneViewportSize.y, 0.0f);

			glm::mat4 transformMatrix = selectedEntity->transformComponent->GetWorldTransformMatrix();
			Matrix cameraViewMatrix;
			cameraViewMatrix = GetCameraMatrix2D(editorCamera);
			ImGuizmo::Manipulate(MatrixToFloat(cameraViewMatrix), glm::value_ptr(projectionMatrix), ImGuizmo::TRANSLATE | ImGuizmo::SCALE | ImGuizmo::ROTATE, ImGuizmo::WORLD, glm::value_ptr(transformMatrix));

			if (ImGuizmo::IsUsing())
			{
				usingImGuizmo = true;
				glm::vec3 skew;
				glm::vec3 pos;
				glm::vec3 scl;
				glm::vec4 pres;
				glm::quat rot;
					
				glm::decompose(selectedEntity->GetParent()? glm::inverse(selectedEntity->GetParent()->transformComponent->GetWorldTransformMatrix()) * transformMatrix : transformMatrix, scl, rot, pos, skew, pres);

				glm::vec3 eulerAngles = glm::eulerAngles(rot);
				eulerAngles = glm::degrees(eulerAngles);

				selectedEntity->transformComponent->position = pos;
				selectedEntity->transformComponent->scale = scl;
				selectedEntity->transformComponent->rotation = eulerAngles;
			}	
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
		
		if (engine->sceneManager->currentScene->mainCamera)
		{
			const RenderTexture* gameViewTex = engine->sceneManager->currentScene->mainCamera->GetRenderTarget();

			rlImGuiImageRenderTextureFit(&gameViewTex->texture, true);
		}
		
		ImGui::End();
	}
	void Editor::UpdateEditor(float deltaTime)
	{
		if (sceneViewportFocused)
		{
			if (SceneType::Scene2D == engine->sceneManager->currentScene->sceneType)
			{
				
				Vector2 mouseWorldPos = GetScreenToWorld2D(Vector2{ EditorInput::GetMousePostion().x, EditorInput::GetMousePostion().y },
					editorCamera);

				if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
				{
					Vector2 delta = GetMouseDelta();
					delta.x *= -1.0f / editorCamera.zoom;
					delta.y *= -1.0f / editorCamera.zoom;
					editorCamera.target.x = editorCamera.target.x + delta.x;
					editorCamera.target.y = editorCamera.target.y + delta.y;
				}


				float wheel = GetMouseWheelMove();
				if (wheel)
				{

					editorCamera.offset = Vector2{ EditorInput::GetMousePostion().x, EditorInput::GetMousePostion().y };
					editorCamera.target = mouseWorldPos;
					float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
					if (wheel < 0) scaleFactor = 1.0f / scaleFactor;
					editorCamera.zoom = glm::clamp(editorCamera.zoom * scaleFactor, 0.005f, 64.0f);
				}
			}
			
		}
		
		
		
			
		glm::vec2 mousePos = EditorInput::GetMousePostion();
		if (mousePos.x < editorCameraRenderTarget.texture.width &&
			mousePos.x > 0 &&
			mousePos.y < editorCameraRenderTarget.texture.height &&
			mousePos.y > 0)
		{
			if (ImGui::IsMouseClicked(MOUSE_BUTTON_LEFT)) 
			{

				BeginTextureMode(colorPickingBuffer);
				BeginMode2D(editorCamera);
				ClearBackground(BLANK);
				BeginShaderMode(colorPickingShader);
				int idUniformLoc = GetShaderLocation(colorPickingShader, "id");
				EndShaderMode();
				for (Entity* entity : engine->sceneManager->currentScene->entities)
				{
					BeginShaderMode(colorPickingShader);
					float id = (float)entity->id;
					SetShaderValue(colorPickingShader, idUniformLoc, (const void*)&id, SHADER_UNIFORM_FLOAT);
					entity->Render();
					entity->ComponentsRender();
					EndShaderMode();
				}
				EndMode2D();
				EndTextureMode();

				Texture t = colorPickingBuffer.texture;
				glm::vec2 mousePos = EditorInput::GetMousePostion();
				Image pickImage = LoadImageFromTexture(t);
				float* pickingData = (float*)pickImage.data;
				int pixelIndex = (int)(editorCameraRenderTarget.texture.height - mousePos.y) * pickImage.width + (int)mousePos.x;
				int id = (int)pickingData[pixelIndex];
				if (usingImGuizmo)
				{
					//do nothing brother.	
				}
				else if(id == 0)
				{
					engine->sceneManager->selectedEntity = nullptr;
				}
				else 
				{
					engine->sceneManager->selectedEntity = engine->sceneManager->currentScene->entityIDtable[id];
				}
				UnloadImage(pickImage);
			}
		}
	}

	void Editor::RenderEditorSceneView()
	{
		//EditorCamera
		if (oldRenderTargetSize != Editor::GetSingleton()->GetSceneViewportSize())
		{
			oldRenderTargetSize = Editor::GetSingleton()->GetSceneViewportSize();
			UnloadRenderTexture(editorCameraRenderTarget);
			UnloadRenderTexture(colorPickingBuffer);
			editorCameraRenderTarget = LoadRenderTexture((int)oldRenderTargetSize.x, (int)oldRenderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			SetTextureFilter(editorCameraRenderTarget.texture, TEXTURE_FILTER_BILINEAR);
			colorPickingBuffer = LoadRenderTexture((int)oldRenderTargetSize.x, (int)oldRenderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R32);
			SetTextureFilter(colorPickingBuffer.texture, TEXTURE_FILTER_POINT);
		}

		BeginTextureMode(editorCameraRenderTarget);
		BeginMode2D(editorCamera);
		glm::vec4 clearColor = engine->sceneManager->currentScene->clearColor;
		Color cc = { (uint8_t)(clearColor.r * 255), (uint8_t)(clearColor.g * 255) , (uint8_t)(clearColor.b * 255) , (uint8_t)(clearColor.a * 255) };
		ClearBackground(cc);

		for (Entity* entity : engine->sceneManager->currentScene->entities)
		{
			entity->Render();
			entity->ComponentsRender();
		}
		//2d scene
		Camera2DEntity* mainCamera = (Camera2DEntity*)engine->sceneManager->currentScene->mainCamera;
		if (mainCamera)
		{

			std::vector<glm::vec2> vertices = {
												{0.0f                                        , mainCamera->GetRenderTarget()->texture.height}, // Bottom-left
												{mainCamera->GetRenderTarget()->texture.width, mainCamera->GetRenderTarget()->texture.height}, // Bottom-right
												{mainCamera->GetRenderTarget()->texture.width, 0.0f}, // Top-right
												{0.0f,                                         0.0f}  // Top-left
			};

			glm::mat4 transform =
				glm::translate(glm::mat4(1.0f), glm::vec3(mainCamera->camera2D.target.x + mainCamera->camera2D.offset.x, mainCamera->camera2D.target.y + +mainCamera->camera2D.offset.y, 0.0f))
				* glm::rotate(glm::mat4(1.0f), -glm::radians(mainCamera->camera2D.rotation), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::mat4(1.0f), glm::vec3((1.0f / mainCamera->camera2D.zoom), (1.0f / mainCamera->camera2D.zoom), 1.0f))
				* glm::translate(glm::mat4(1.0f), -glm::vec3(mainCamera->camera2D.target.x + mainCamera->camera2D.offset.x, mainCamera->camera2D.target.y + mainCamera->camera2D.offset.y, 0.0f))
				* glm::translate(glm::mat4(1.0f), glm::vec3(mainCamera->camera2D.target.x + mainCamera->camera2D.offset.x, mainCamera->camera2D.target.y + mainCamera->camera2D.offset.y, 0.0f));

			std::vector<glm::vec2> transformedVertices;
			for (const auto& vertex : vertices) {
				glm::vec4 transformedVertex = transform * glm::vec4(vertex, 0.0f, 1.0f);
				transformedVertices.push_back(glm::vec2(transformedVertex));
			}


			DrawLine((int)transformedVertices[0].x, (int)transformedVertices[0].y, (int)transformedVertices[1].x, (int)transformedVertices[1].y, GRAY);
			DrawLine((int)transformedVertices[1].x, (int)transformedVertices[1].y, (int)transformedVertices[2].x, (int)transformedVertices[2].y, GRAY);
			DrawLine((int)transformedVertices[2].x, (int)transformedVertices[2].y, (int)transformedVertices[3].x, (int)transformedVertices[3].y, GRAY);
			DrawLine((int)transformedVertices[3].x, (int)transformedVertices[3].y, (int)transformedVertices[0].x, (int)transformedVertices[0].y, GRAY);
		}
		EndMode2D();
		EndTextureMode();
	}
}
