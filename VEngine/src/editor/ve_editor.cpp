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
namespace VE 
{

	std::vector<std::string>* logs = new std::vector<std::string>();
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


		colorPickingShader = LoadShader(0, "resources/shaders/color_picking_shader.fs");
		editorCamera = {};
		editorCamera.zoom = 1.0f;


		entityName.resize(255);
		memset(entityName.data(), 0, entityName.size());
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

		consoleWindow.Draw();

		ImGui::PopFont();
		rlImGuiEnd();
		//make sure this gets called after DrawSceneViewport().
		UpdateEditor(GetFrameTime());
		
	}
	
	void Editor::AddChildrenNode(flecs::entity child) 
	{
		ImGuiTreeNodeFlags flags = selectedEntity == (child) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(child)), flags, child.name().c_str());

		if (ImGui::IsItemClicked())
		{
			selectedEntity = child;
		}

		if (ImGui::BeginPopupContextItem(0, 1))
		{
			selectedEntity = child;
			if (ImGui::MenuItem("Add Child"))
			{
				flecs::entity _child = engine->sceneManager->currentScene->AddEntity("EmptyEntity");
				_child.child_of(child);
			}
			if (ImGui::MenuItem("Remove"))
			{
				child.destruct();
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
		if (ImGui::IsItemClicked())
		{
			selectedEntity = e;
		}
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			selectedEntity = e;
			if (ImGui::MenuItem("Add Child"))
			{
				flecs::entity child = engine->sceneManager->currentScene->AddEntity("EmptyEntity");
				child.child_of(e);
			}
			if (ImGui::MenuItem("Remove"))
			{
				e.destruct();
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
					engine->sceneManager->currentScene->AddEntity("EmptyEntity");
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Add Construct"))
			{

			}
			ImGui::EndPopup();
		}

		flecs::query sceneEntitiesQuery = engine->sceneManager->currentScene->world.query_builder().with<_Components::SceneTag>().build();

		engine->sceneManager->currentScene->world.defer_begin();
		sceneEntitiesQuery.each([&](flecs::entity e) 
			{
				
				AddEntityNode(e);
			});
		engine->sceneManager->currentScene->world.defer_end();


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
					std::filesystem::path relativePath = scenePath.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().generic_string() + "/assets");
					engine->sceneManager->LoadScene(relativePath);
					selectedEntity = flecs::entity();
				}
			}
			if (ImGui::BeginMenu("New Scene"))
			{
				if (ImGui::MenuItem("2D scene"))
				{
					std::filesystem::path scenePath = VE::SaveFileDialog();
					if (!scenePath.empty())
					{
						std::filesystem::path relativePath = scenePath.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().generic_string() + "/assets");
						engine->sceneManager->LoadScene(relativePath);
						selectedEntity = flecs::entity();
					}

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
				//if (!engine->sceneManager->currentScene->scenePath.empty())
				//{
					engine->sceneManager->mode = SceneMode::Game;
					std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
					engine->ReloadProjectSharedLibrary();
					engine->sceneManager->LoadScene(reloadScenePath);
					ImGui::SetWindowFocus("GameViewport");
					selectedEntity = flecs::entity();
				//}

			}

			ImGui::SameLine();

			if (ImGui::Button("Reload Project"))
			{
				engine->sceneManager->SaveScene();
				std::filesystem::path reloadScenePath = engine->sceneManager->currentScene->scenePath;
				engine->ReloadProjectSharedLibrary();
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
				if (!engine->sceneManager->currentScene->world.lookup(entityName.data()) && saveName)
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
		ImGui::End();
	}

	void Editor::DrawStatus()
	{
		ImGui::Begin("Status");
		float frameTime = GetFrameTime();
		float fps = 1.0f / frameTime;
		ImGui::Text("Frame Time  : %fms", frameTime);
		ImGui::Text("FPS         : %f", fps);
		
		ImGui::End();
	}

	void Editor::DrawSceneViewport()
	{
		ImGui::Begin("SceneViewport");
		
		ImGui::End();
	}

	void Editor::DrawGameViewport()
	{
		ImVec2 oldPadding = ImGui::GetStyle().WindowPadding;
		ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
		ImGui::Begin("GameViewport");
		
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

			const Texture* rt = &c2dc->renderTarget.texture;

			rlImGuiImageRenderTextureFit(rt, true);

		}


		ImGui::End();
		ImGui::GetStyle().WindowPadding = oldPadding;
	}

	void Editor::UpdateEditor(float deltaTime)
	{
		
	}

	void Editor::RenderEditorSceneView()
	{
		
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
		if (logs->size() > 512) 
		{
			logs->erase(logs->begin());
		}
		logs->push_back(message);
		scrollToBottom = true;
	}
}
