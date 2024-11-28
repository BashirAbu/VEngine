#include "ve_editor.h"
#include "ve_editor_input.h"
#include "ve_engine.h"
#include <rlImGui.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include "platform/ve_platform.h"
#include "editor/ve_editor_elements.h"
#include "imgui_impl_raylib.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include "utils/ve_utils.h"
#include <raymath.h>
#include <mutex>
namespace VE 
{
	std::vector<std::string>* logs = nullptr;
	std::mutex logsMutex;
	bool scrollToBottom = false;
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
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		
		GrayTheme();

		font = io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans-Medium.ttf", 18.0f);
		consoleFont = io.Fonts->AddFontFromFileTTF("resources/fonts/SourceCodePro-Medium.ttf", 16.0f);

		// required to be called to cache the font texture with raylib
		io.Fonts->Build();
		ImGui_ImplRaylib_BuildFontAtlas();

		editorCamera = {};
		editorCamera.zoom = 1.0f;

		entityName.resize(255);
		memset(entityName.data(), 0, entityName.size());

		editorCameraRenderTarget = LoadRenderTexture((int)sceneViewportSize.x, (int)sceneViewportSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		colorPickingBuffer = LoadRenderTexture((int)sceneViewportSize.x, (int)sceneViewportSize.y, PIXELFORMAT_UNCOMPRESSED_R32);
		SetTextureFilter(colorPickingBuffer.texture, TEXTURE_FILTER_POINT);

		colorPickingShader = LoadShader(0, "resources/shaders/color_picking_shader.fs");
	}
	Editor::~Editor() 
	{
		UnloadShader(colorPickingShader);
		UnloadRenderTexture(editorCameraRenderTarget);
		UnloadRenderTexture(colorPickingBuffer);
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

		//ImGui::ShowDemoWindow();

		DrawMainMenuBar();

		DrawHierarchy();

		DrawInspector();

		DrawStatus();

		DrawSceneViewport();

		DrawGameViewport();

		consoleWindow.Draw();

		//ImGui::Begin("Pick");

		//rlImGuiImageRenderTextureFit(&colorPickingBuffer.texture, true);

		//ImGui::End();

		ImGui::PopFont();
		rlImGuiEnd();
		//make sure this gets called after DrawSceneViewport().
		UpdateEditor(GetFrameTime());
	}

	void Editor::AddChildrenNode(flecs::entity child) 
	{
		ImGuiTreeNodeFlags flags = selectedEntity == (child) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(child)), flags, child.name().c_str());

		if (ImGui::BeginDragDropSource())
		{
			void* payload = &child;
			ImGui::SetDragDropPayload("ENTITY", payload, sizeof(child), ImGuiCond_Once);

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked())
		{
			selectedEntity = child;
		}

		if (ImGui::BeginPopupContextItem(0, 1))
		{
			selectedEntity = child;
			if (ImGui::MenuItem("Add Child"))
			{
				addChildQueue.push(child);
			}
			if (ImGui::MenuItem("Remove"))
			{
				removeEntities.push(child);
			}
			if (ImGui::MenuItem("Duplicate"))
			{
				//engine->sceneManager->currentScene->CloneEntity(selectedEntity);
				cloneEntities.push(child);
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			child.children([&](flecs::entity e)
				{
					AddChildrenNode(e);
				});
			ImGui::TreePop();
		}
	}

	

	void Editor::AddEntityNode(flecs::entity e)
	{
		if (e.parent())
		{
			return;
		}
		ImGuiTreeNodeFlags flags = ((selectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)((uint64_t)e), flags, e.name().c_str());


		if (ImGui::BeginDragDropSource())
		{
			void* payload = &e;
			ImGui::SetDragDropPayload("ENTITY", payload, sizeof(e), ImGuiCond_Once);

			ImGui::EndDragDropSource();
		}


		if (ImGui::IsItemClicked())
		{
			selectedEntity = e;
		}
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			selectedEntity = e;
			if (ImGui::MenuItem("Add Child"))
			{
				addChildQueue.push(e);
			}
			if (ImGui::MenuItem("Remove"))
			{
				removeEntities.push(e);
			}
			if (ImGui::MenuItem("Make Construct"))
			{
				std::string constructJson = engine->sceneManager->currentScene->SerializeEntity(e);
				constructJson = "{ \"results\":[" + constructJson + "]}";
				std::filesystem::path constructPath = SaveFileDialog(VE_CONSTRUCT_FILE_EXTENSION);

				std::ofstream constructFile(constructPath);

				constructFile << constructJson;

				constructFile.close();
				
			}
			if (ImGui::MenuItem("Duplicate"))
			{
				cloneEntities.push(e);
			}

			ImGui::EndPopup();
		}
		if (opened)
		{

			e.children([&](flecs::entity e)
				{
					AddChildrenNode(e);
				});

			ImGui::TreePop();
		}
	}
	void Editor::DrawHierarchy()
	{

		ImGui::Begin("Hierarchy");

		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::BeginMenu("Add Entity"))
			{
				if (ImGui::MenuItem("Empty Entity"))
				{
					addEntities.push("EmptyEntity");
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Add Construct"))
			{
				std::filesystem::path relativePath = GetRelativePath(OpenFileDialog());
				engine->sceneManager->currentScene->AddConstruct(relativePath);
			}
			ImGui::EndPopup();
		}
		flecs::query sceneEntitiesQuery = engine->sceneManager->currentScene->world.query_builder().with<_Components::SceneEntityTag>().build();
		sceneEntitiesQuery.each([&](flecs::entity e) 
			{
				std::string name = e.name().c_str();
				AddEntityNode(e);
			});

		while (!addEntities.empty()) 
		{
			engine->sceneManager->currentScene->AddEntity(addEntities.front());
			addEntities.pop();
		}
		while (!cloneEntities.empty()) 
		{
			engine->sceneManager->currentScene->CloneEntity(cloneEntities.front());
			cloneEntities.pop();
		}
		while (!removeEntities.empty())
		{
			removeEntities.front().destruct();
			removeEntities.pop();
		}
		while (!addChildQueue.empty())
		{
			engine->sceneManager->currentScene->AddEntity("EmptyEntity").child_of(addChildQueue.front());
			addChildQueue.pop();
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
					engine->sceneManager->LoadScene(GetRelativePath(scenePath));
					selectedEntity = flecs::entity();
				}
			}
			if (ImGui::BeginMenu("New Scene"))
			{
				if (ImGui::MenuItem("2D scene"))
				{
					//std::filesystem::path scenePath = VE::SaveFileDialog();
					
					//engine->sceneManager->LoadScene(GetRelativePath(scenePath));
					engine->sceneManager->UnloadScene();
					engine->sceneManager->currentScene = new Scene(SceneType::Scene2D);
					selectedEntity = flecs::entity();
					
				}
				if (ImGui::MenuItem("3D scene"))
				{
					//handle 3d scene creation.
					selectedEntity = flecs::entity();
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
					engine->sceneManager->LoadScene(reloadScenePath);
					ImGui::SetWindowFocus("GameViewport");
					selectedEntity = flecs::entity();
				}

			}

			ImGui::SameLine();

			if (ImGui::Button("Reload Project"))
			{
				engine->sceneManager->SaveScene();
				std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;

				engine->sceneManager->LoadScene(reloadScenePath);
				selectedEntity = flecs::entity();
			}
		}
		else
		{
			if (ImGui::Button("End"))
			{
				engine->sceneManager->mode = SceneMode::Editor;
				std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
				engine->sceneManager->LoadScene(reloadScenePath);
				ImGui::SetWindowFocus("SceneViewport");
				selectedEntity = flecs::entity();
			}
		}
		
		ImGui::EndMainMenuBar();
	}
	
	void Editor::DrawInspector()
	{
		ImGui::Begin("Inspector");
		
		if (selectedEntity)
		{
			ImGui::Text("Name");
			ImGui::SameLine();
			ImGui::InputText("##Name", entityName.data(), entityName.size());
			if (ImGui::IsItemActivated())
			{
				saveName = true;
			}
			else if (ImGui::IsItemDeactivated())
			{
				if (!engine->sceneManager->currentScene->LookupEntity(entityName.data()) && saveName)
				{
					selectedEntity.set_name(entityName.data());
					saveName = false;
				}
			}
			else 
			{
				strcpy(entityName.data(), selectedEntity.name().c_str());
			}
			
			ImGui::Separator();

			selectedEntity.each([&](flecs::id compId) 
			{
				if (compId.is_entity())
				{
					if (engine->sceneManager->currentScene->componentsTable.find(compId.entity().name().c_str()) != engine->sceneManager->currentScene->componentsTable.end())
					{
						DrawComponentElements(compId.entity().name().c_str(), selectedEntity);
						ImGui::Separator();
					}
				}
			});

			ImGuiStyle& style = ImGui::GetStyle();

			float size = ImGui::CalcTextSize("Add Component").x + style.FramePadding.x * 2.0f;
			float avail = ImGui::GetContentRegionAvail().x;

			float off = (avail - size) * .5f;
			if (off > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Add Component");

			}
			if (ImGui::BeginPopup("Add Component"))
			{
				for (auto comp : engine->sceneManager->currentScene->componentsTable) 
				{
					if (ImGui::MenuItem(comp.first.c_str()))
					{
						selectedEntity.add(comp.second);
					}
					ImGui::Separator();
				}
				ImGui::EndPopup();
			}
		}
		else
		{
			//stuff in scenes.
			ImGui::Text("Scene Systems:");
			for (auto& system : engine->sceneManager->currentScene->systemsTable)
			{
				ImGui::Text("%s", system.first.c_str());
				ImGui::SameLine();
				ImGui::Checkbox((std::string("##chebox_") + system.first).c_str(), &system.second.enable);
				if (ImGui::IsItemEdited())
				{
					flecs::system* s = (flecs::system*)&system.second.entity;
					if (system.second.enable)
					{
						s->enable();
					}
					else 
					{
						s->disable();
					}
				}
			}
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
		ImGui::Text("Entity Count: %d", (int)engine->sceneManager->currentScene->world.count<_Components::SceneEntityTag>());
		if (selectedEntity)
		{
			ImGui::Text("Selected Entity Name: %s. ID: %d", selectedEntity.name().c_str(), (int)selectedEntity.id());
		}

		ImGui::End();
	}

	void Editor::DrawSceneViewport()
	{
		ImGui::Begin("SceneViewport");
		sceneViewportFocused = ImGui::IsWindowFocused();
		isSceneViewHovered = ImGui::IsWindowHovered();
		ImVec2 vec2 = ImGui::GetContentRegionAvail();
		sceneViewportSize = *(glm::vec2*) & vec2;
		vec2 = ImGui::GetCursorScreenPos();
		sceneViewportPosition = *(glm::vec2*)&vec2;

		if (glm::vec2(editorCameraRenderTarget.texture.width, editorCameraRenderTarget.texture.height) != sceneViewportSize)
		{
			UnloadRenderTexture(editorCameraRenderTarget);
			editorCameraRenderTarget = LoadRenderTexture((int)sceneViewportSize.x, (int)sceneViewportSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			SetTextureFilter(editorCameraRenderTarget.texture, TEXTURE_FILTER_BILINEAR);
		}

		BeginTextureMode(editorCameraRenderTarget);
		BeginMode2D(editorCamera);

		flecs::query cameras = engine->sceneManager->currentScene->world.query<Components::Camera2DComponent>();
		Components::Camera2DComponent* c2dc = nullptr;

		cameras.each([&](flecs::entity e, Components::Camera2DComponent& cc)
			{
				if (cc.isMain)
				{
					c2dc = &cc;
				}
			});
		if (c2dc)
		{
			ClearBackground(GLMVec4ToRayColor(c2dc->backgroundColor));
		}
		else 
		{
			ClearBackground(GLMVec4ToRayColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		}

		engine->sceneManager->currentScene->renderer.RenderQueued();
		

		//Draw Camrea rectangle
		if (c2dc)
		{
			std::vector<glm::vec2> vertices = {
									{0.0f                                        , c2dc->renderTarget.texture.height}, // Bottom-left
									{c2dc->renderTarget.texture.width            , c2dc->renderTarget.texture.height}, // Bottom-right
									{c2dc->renderTarget.texture.width, 0.0f}     , // Top-right
									{0.0f,                                         0.0f}  // Top-left
			};

			glm::mat4 transform =
				glm::translate(glm::mat4(1.0f), glm::vec3(c2dc->camera.target.x + c2dc->camera.offset.x, c2dc->camera.target.y + c2dc->camera.offset.y, 0.0f))
				* glm::rotate(glm::mat4(1.0f), -glm::radians(c2dc->camera.rotation), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::mat4(1.0f), glm::vec3((1.0f / c2dc->camera.zoom), (1.0f / c2dc->camera.zoom), 1.0f))
				* glm::translate(glm::mat4(1.0f), -glm::vec3(c2dc->camera.target.x + c2dc->camera.offset.x, c2dc->camera.target.y + c2dc->camera.offset.y, 0.0f))
				* glm::translate(glm::mat4(1.0f), glm::vec3(c2dc->camera.target.x + c2dc->camera.offset.x, c2dc->camera.target.y + c2dc->camera.offset.y, 0.0f));

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
		rlImGuiImageRenderTextureFit(&editorCameraRenderTarget.texture, false);
		//Draw ImGuizmo stuff here.
		ImGuizmo::BeginFrame();
		usingImGuizmo = false;
		if (selectedEntity)
		{
			Components::TransformComponent* tc = selectedEntity.get_mut<Components::TransformComponent>();
			if (tc) 
			{
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(sceneViewportPosition.x, sceneViewportPosition.y, sceneViewportSize.x, sceneViewportSize.y);
				glm::mat4 projectionMatrix = glm::ortho(0.0f, sceneViewportSize.x, sceneViewportSize.y, 0.0f);

				glm::mat4 transformMatrix = tc->__worldMatrix;
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

					std::string parentName = selectedEntity.parent() ? selectedEntity.parent().name().c_str() : "no";

					glm::decompose(selectedEntity.parent()? glm::inverse(selectedEntity.parent().get_mut<Components::TransformComponent>()->__worldMatrix) * transformMatrix : transformMatrix, scl, rot, pos, skew, pres);
					glm::vec3 eulerAngles = glm::eulerAngles(rot);
					eulerAngles = glm::degrees(eulerAngles);

					tc->localPosition = pos;
					tc->localScale = scl;
					tc->localRotation = eulerAngles;
				}
			}
		}
		ImGui::End();
	}

	void Editor::DrawGameViewport()
	{
		ImVec2 oldPadding = ImGui::GetStyle().WindowPadding;
		ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
		ImGui::Begin("GameViewport");

		ImVec2 vec2 = ImGui::GetContentRegionAvail();
		gameViewportSize = *((glm::vec2*)&vec2);

		gameViewportFocused = ImGui::IsWindowFocused();

		flecs::query cameras = engine->sceneManager->currentScene->world.query<Components::Camera2DComponent>();
		
		Components::Camera2DComponent* c2dc = nullptr;

		cameras.each([&](flecs::entity e, Components::Camera2DComponent& cc) 
			{
				if (cc.isMain) 
				{
					c2dc = &cc;
				}
			});

		if (c2dc)
		{
			ImVec2 area = ImGui::GetContentRegionAvail();

			float scale = area.x / c2dc->renderTarget.texture.width;

			float y = c2dc->renderTarget.texture.height * scale;
			if (y > area.y)
			{
				scale = area.y / c2dc->renderTarget.texture.height;
			}

			int sizeY = int(c2dc->renderTarget.texture.height * scale);

			vec2 = ImGui::GetCursorScreenPos();
			vec2.y = (area.y / 2 - sizeY / 2) + (vec2.y - .5f);
			gameViewportPosition = *((glm::vec2*)&vec2);

			const Texture* rt = &c2dc->renderTarget.texture;

			rlImGuiImageRenderTextureFit(rt, true);

		}
		ImGui::End();
		ImGui::GetStyle().WindowPadding = oldPadding;
	}

	void Editor::UpdateEditor(float deltaTime)
	{
		if (sceneViewportFocused)
		{
			if (SceneType::Scene2D == engine->sceneManager->currentScene->sceneType)
			{
				Vector2 mouseWorldPos = GetScreenToWorld2D(Vector2{ EditorInput::GetMousePostion().x, EditorInput::GetMousePostion().y}, editorCamera);
				if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
				{
					Vector2 delta = GetMouseDelta();
					delta.x *= -1.0f / editorCamera.zoom;
					delta.y *= -1.0f / editorCamera.zoom;
					editorCamera.target.x = editorCamera.target.x + delta.x;
					editorCamera.target.y = editorCamera.target.y + delta.y;
				}
				float wheel = 0;
				if (isSceneViewHovered)
				{
					wheel = GetMouseWheelMove();
				}
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

		if (IsSceneViewportFocused() && isSceneViewHovered)
		{
			if (ImGui::IsMouseClicked(MOUSE_BUTTON_LEFT))
			{

				if (glm::vec2(colorPickingBuffer.texture.width, colorPickingBuffer.texture.height) != sceneViewportSize)
				{
					UnloadRenderTexture(colorPickingBuffer);
					colorPickingBuffer = LoadRenderTexture((int)sceneViewportSize.x, (int)sceneViewportSize.y, PIXELFORMAT_UNCOMPRESSED_R32);
					SetTextureFilter(colorPickingBuffer.texture, TEXTURE_FILTER_POINT);
				}


				BeginTextureMode(colorPickingBuffer);
				BeginMode2D(editorCamera);
				ClearBackground(BLANK);
				BeginShaderMode(colorPickingShader);
				int idUniformLoc = GetShaderLocation(colorPickingShader, "id");
				EndShaderMode();

				for(const auto& tex2d : engine->sceneManager->currentScene->renderer.texture2DRenderQueue)
				{
					if (tex2d.entity)
					{
						BeginShaderMode(colorPickingShader);
						float id = (float)((int)tex2d.entity);
						SetShaderValue(colorPickingShader, idUniformLoc, (const void*)&id, SHADER_UNIFORM_FLOAT);
						DrawTexturePro(*tex2d.texture.texture, tex2d.texture.source, tex2d.texture.dest, tex2d.texture.origin, tex2d.texture.rotation, tex2d.texture.tint);
						EndShaderMode();
					}
					
				}
				EndMode2D();
				EndTextureMode();

				Texture t = colorPickingBuffer.texture;

				Image pickImage = LoadImageFromTexture(t);
				float* pickingData = (float*)pickImage.data;
				glm::vec2 mousePos = EditorInput::GetMousePostion();
				int pixelIndex = (int)(pickImage.height - mousePos.y) * pickImage.width + (int)mousePos.x;
				
				int id = (int)pickingData[pixelIndex < pickImage.width * pickImage.height ? pixelIndex : 0];
				if (usingImGuizmo)
				{
					//do nothing brother.	
				}
				else if (id)
				{
					selectedEntity = engine->sceneManager->currentScene->LookupEntity((flecs::entity)id);
				}
				else 
				{
					selectedEntity = flecs::entity();
				}
				UnloadImage(pickImage);
			}
		}
	}

	Editor::ConsoleWindow::ConsoleWindow()
	{

	}
	Editor::ConsoleWindow::~ConsoleWindow()
	{
	}

	void Editor::ConsoleWindow::Draw()
	{
		ImGui::Begin("Console");

		if (ImGui::Button("Clear"))
		{
			ClearLog();
		}

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
		ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AllowTabInput;
		ImGui::PushFont(VE::Engine::GetSingleton()->editor->consoleFont);
		for (const auto log : *logs)
		{
			ImColor color = ImColor(0.1f, 0.1f, 0.1f, 1.0f);
			if (log.find("[INFO]") != std::string::npos)
			{
				color = ImColor(0.1f, 0.1f, 0.1f, 1.0f);
			}
			else if (log.find("[ERROR]") != std::string::npos)
			{
				color = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (log.find("[FATAL]") != std::string::npos)
			{
				color = ImColor(0.5f, 0.0f, 0.0f, 1.0f);
			}
			else if (log.find("[WARN]") != std::string::npos)
			{
				color = ImColor(7.0f, 7.0f, 0.0f, 1.0f);
			}
			else if (log.find("[DEBUG]") != std::string::npos)
			{
				color = ImColor(0.0f, .7f, 0.0f, 1.0f);
			}
			ImGui::TextColored(color, log.c_str());
		}
		ImGui::PopFont();
		if (scrollToBottom) 
		{
			ImGui::SetScrollHereY(1.0f);
			scrollToBottom = false;
		}

		ImGui::EndChild();
		ImGui::End();
	}
	void Editor::ConsoleWindow::ClearLog()
	{
		logs->clear();
	}
	void AddLog(const std::string message)
	{
		std::lock_guard<std::mutex> lock(logsMutex);
		if (!logs)
		{
			logs = new std::vector<std::string>();
		}
		if (logs->size() > 50) 
		{
			logs->erase(logs->begin());
		}
		logs->push_back(message);
		scrollToBottom = true;
	}
}
