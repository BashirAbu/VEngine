#ifdef VE_EDITOR
#include "ve_editor.h"
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
#include <rlgl.h>
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
	void SetDarkThemeColors()
	{
		// Get style and colors references
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// Base color definitions for clarity
		ImVec4 black = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		ImVec4 darkGray = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		ImVec4 midGray = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		ImVec4 lightGray = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		ImVec4 accentColor = ImVec4(0.36f, 0.50f, 0.75f, 1.00f); // bluish accent
		ImVec4 accentHovered = ImVec4(0.46f, 0.60f, 0.85f, 1.00f);
		ImVec4 accentActive = ImVec4(0.26f, 0.40f, 0.65f, 1.00f);

		// Background and window colors
		colors[ImGuiCol_WindowBg] = darkGray;
		colors[ImGuiCol_ChildBg] = ImVec4(darkGray.x, darkGray.y, darkGray.z, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(midGray.x, midGray.y, midGray.z, 0.95f);
		colors[ImGuiCol_Border] = ImVec4(lightGray.x, lightGray.y, lightGray.z, 0.10f);
		colors[ImGuiCol_BorderShadow] = ImVec4(black.x, black.y, black.z, 0.00f);

		// Frame colors (like input boxes, checkboxes, etc.)
		colors[ImGuiCol_FrameBg] = midGray;
		colors[ImGuiCol_FrameBgHovered] = ImVec4(midGray.x + 0.05f, midGray.y + 0.05f, midGray.z + 0.05f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(midGray.x + 0.10f, midGray.y + 0.10f, midGray.z + 0.10f, 1.00f);

		// Title bar colors
		colors[ImGuiCol_TitleBg] = ImVec4(darkGray.x, darkGray.y, darkGray.z, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(darkGray.x, darkGray.y, darkGray.z, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(darkGray.x, darkGray.y, darkGray.z, 0.75f);

		// Menubar/Toolbar
		colors[ImGuiCol_MenuBarBg] = ImVec4(darkGray.x + 0.05f, darkGray.y + 0.05f, darkGray.z + 0.05f, 1.00f);

		// Scrollbar
		colors[ImGuiCol_ScrollbarBg] = ImVec4(darkGray.x, darkGray.y, darkGray.z, 0.60f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(midGray.x, midGray.y, midGray.z, 0.90f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(midGray.x + 0.05f, midGray.y + 0.05f, midGray.z + 0.05f, 0.90f);
		colors[ImGuiCol_ScrollbarGrabActive] = accentActive;

		// Check marks, sliders, etc.
		colors[ImGuiCol_CheckMark] = accentColor;
		colors[ImGuiCol_SliderGrab] = accentColor;
		colors[ImGuiCol_SliderGrabActive] = accentActive;

		// Buttons
		colors[ImGuiCol_Button] = ImVec4(midGray.x, midGray.y, midGray.z, 1.00f);
		colors[ImGuiCol_ButtonHovered] = accentHovered;
		colors[ImGuiCol_ButtonActive] = accentActive;

		// Header (for collapsing headers, selectable texts)
		colors[ImGuiCol_Header] = ImVec4(midGray.x, midGray.y, midGray.z, 1.00f);
		colors[ImGuiCol_HeaderHovered] = accentHovered;
		colors[ImGuiCol_HeaderActive] = accentActive;

		// Tabs
		colors[ImGuiCol_Tab] = midGray;
		colors[ImGuiCol_TabHovered] = accentHovered;
		colors[ImGuiCol_TabActive] = accentColor;
		colors[ImGuiCol_TabUnfocused] = darkGray;
		colors[ImGuiCol_TabUnfocusedActive] = midGray;

		// Separators and resizing handles
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = accentHovered;
		colors[ImGuiCol_SeparatorActive] = accentActive;
		colors[ImGuiCol_ResizeGrip] = ImVec4(midGray.x + 0.1f, midGray.y + 0.1f, midGray.z + 0.1f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = accentHovered;
		colors[ImGuiCol_ResizeGripActive] = accentActive;

		// Plot lines and histograms
		colors[ImGuiCol_PlotLines] = lightGray;
		colors[ImGuiCol_PlotLinesHovered] = accentColor;
		colors[ImGuiCol_PlotHistogram] = accentColor;
		colors[ImGuiCol_PlotHistogramHovered] = accentHovered;

		// Text
		colors[ImGuiCol_Text] = lightGray;
		colors[ImGuiCol_TextDisabled] = ImVec4(lightGray.x, lightGray.y, lightGray.z, 0.50f);

		// Modal window darkening
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(black.x, black.y, black.z, 0.60f);
		colors[ImGuiCol_NavHighlight] = accentColor;
		colors[ImGuiCol_NavWindowingHighlight] = accentColor;
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(black.x, black.y, black.z, 0.60f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(darkGray.x, darkGray.y, darkGray.z, 1.00f);

		// Style adjustments
		style.WindowRounding = 5.0f;
		style.FrameRounding = 3.0f;
		style.GrabRounding = 3.0f;
		style.PopupRounding = 3.0f;
		style.TabRounding = 3.0f;
		style.ScrollbarRounding = 3.0f;

		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.FramePadding = ImVec2(5.0f, 3.0f);
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.IndentSpacing = 20.0f;
	}

	Editor* Editor::singleton = nullptr;


	Editor::Editor(class Engine* engine) : engine(engine)
	{
		singleton = this;
		rlImGuiSetup(true);

		ImGuiIO& io = (ImGui::GetIO());
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable docking
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		
		SetDarkThemeColors();
		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Latin
			0x0600, 0x06FF, // Arabic
			0x0750, 0x077F, // Arabic Supplement
			0x08A0, 0x08FF, // Arabic extended
			0xFB50, 0xFDFF, // Arabic Presentation Forms-A
			0xFE70, 0xFEFF, // Arabic Presentation Forms-B
			0x0590, 0x05FF, // Hebrew
			0
		};

		font = io.Fonts->AddFontFromFileTTF("resources/fonts/GoNotoKurrent-Regular.ttf", 18.0f, nullptr, &ranges[0]);
		consoleFont = io.Fonts->AddFontFromFileTTF("resources/fonts/SourceCodePro-Medium.ttf", 16.0f);

		// required to be called to cache the font texture with raylib
		io.Fonts->Build();
		ImGui_ImplRaylib_BuildFontAtlas();

		editorCamera2D = {};
		editorCamera2D.zoom = 1.0f;

		editorCamera3D = {};
		editorCamera3D.fovy = 45.0f;
		editorCamera3D.position = Vector3{0.0f, 10.0f, 10.0f};
		editorCamera3D.projection = CAMERA_PERSPECTIVE;
		editorCamera3D.target = {};
		editorCamera3D.up = { 0.0f, 1.0f, 0.0f };

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

		if (ImGui::BeginDragDropTarget())
		{

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY");

			if (payload)
			{
				const flecs::entity* dropped = (const flecs::entity*)payload->Data;

				std::function<void(flecs::entity parent, flecs::entity possibleChild)> checkDescendants;

				bool result = false;
				checkDescendants = [&](flecs::entity possibleChild, flecs::entity possibleParent)
					{
						if (possibleParent == possibleChild)
						{
							result =  true;
						}
						possibleParent.children([&](flecs::entity c)
							{
								if (c == possibleChild)
								{
									result = true;
								}
								checkDescendants(possibleChild, c);
							});
					};
				checkDescendants(child, *dropped);
				if (!result)
				{
					dropped->remove(flecs::ChildOf);
					dropped->child_of(child);
				}
			}

			ImGui::EndDragDropTarget();
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

		if (ImGui::BeginDragDropTarget())
		{

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY");

			if (payload)
			{
				const flecs::entity* dropped = (const flecs::entity*)payload->Data;
				dropped->remove(flecs::ChildOf);
				dropped->child_of(e);
			}

			ImGui::EndDragDropTarget();
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
				nlohmann::ordered_json constructJsonWrapper;
				nlohmann::ordered_json constructEntList = nlohmann::ordered_json::parse("[" + constructJson + "]");

				constructJsonWrapper["entities"] = constructEntList;
				nlohmann::ordered_json relations;
				nlohmann::ordered_json root;
				for (auto& entJson : constructJsonWrapper["entities"])
				{
					if (entJson.contains("parent"))
					{
						std::string parentPath = entJson["parent"];
						std::size_t pos = parentPath.rfind(".");
						std::string parentName = (pos != std::string::npos) ? parentPath.substr(pos + 1) : parentPath;

						relations[(std::string)entJson["name"]] = parentName;
						entJson.erase("parent");
					}
					else 
					{
						root["root"] = entJson["name"];
					}
				}
				constructJsonWrapper["relations"] = relations;
				constructJsonWrapper["root"] = root["root"];

				std::filesystem::path constructPath = SaveFileDialog(VE_CONSTRUCT_FILE_EXTENSION);

				std::ofstream constructFile(constructPath);

				constructFile << constructJsonWrapper;

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
		engine->sceneManager->currentScene->world.defer_begin();
		sceneEntitiesQuery.each([&](flecs::entity e) 
			{
				std::string name = e.name().c_str();
				AddEntityNode(e);
			});
		engine->sceneManager->currentScene->world.defer_end();


		ImGui::SetCursorPos(ImGui::GetStyle().FramePadding);
		ImGui::Dummy(ImGui::GetContentRegionAvail());


		if (ImGui::BeginDragDropTarget())
		{

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY");

			if (payload)
			{
				const flecs::entity* dropped = (const flecs::entity*)payload->Data;
				if (dropped->parent())
				{
					dropped->remove(flecs::ChildOf, dropped->parent());
				}
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow))
		{
			selectedEntity = flecs::entity();
		}


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
					tempSelecetedEntity = selectedEntity ? selectedEntity.name().c_str() : "_398743984_k;dslajf;kds";
					engine->sceneManager->mode = SceneMode::Game;

					reloadScenePath = engine->sceneManager->currentScene->scenePath;
					
					engine->sceneManager->LoadScene(reloadScenePath);
					ImGui::SetWindowFocus("GameViewport");
					selectedEntity = engine->sceneManager->currentScene->_LookupEntity(tempSelecetedEntity.c_str());

				}

			}

			ImGui::SameLine();

			if (ImGui::Button("Reload Project"))
			{
				tempSelecetedEntity = selectedEntity ? selectedEntity.name().c_str() : "_398743984_k;dslajf;kds";
				engine->sceneManager->SaveScene();
				reloadScenePath = engine->sceneManager->currentScene->scenePath;
				engine->sceneManager->LoadScene(reloadScenePath);
				selectedEntity = flecs::entity();
				selectedEntity = engine->sceneManager->currentScene->_LookupEntity(tempSelecetedEntity.c_str());

			}
		}
		else
		{
			if (ImGui::Button("End"))
			{
				engine->sceneManager->mode = SceneMode::Editor;
				engine->sceneManager->LoadScene(reloadScenePath);
				ImGui::SetWindowFocus("SceneViewport");
				selectedEntity = flecs::entity();
				selectedEntity = engine->sceneManager->currentScene->_LookupEntity(tempSelecetedEntity.c_str());
			}
		}
		ImGui::EndMainMenuBar();
	}
	
	void Editor::DrawInspector()
	{
		ImGui::Begin("Inspector");
		if (selectedEntity)
		{
			ImGui::SameLine();

			bool enabled = !selectedEntity.has<_Components::Disabled>();

			ImGui::Checkbox(((std::string)"##" + selectedEntity.name().c_str()).c_str(), &enabled);

			std::function<void(flecs::entity child, bool enable)> enableChildren;

			enableChildren = [&](flecs::entity e, bool enable) -> void
				{
					
					if (!enable)
					{
						e.add< _Components::Disabled>();
					}
					else 
					{
						e.remove< _Components::Disabled>();
					}
					e.children([&](flecs::entity c)
						{
							if (!enable)
							{
								c.add<_Components::Disabled>();
							}
							else
							{
								c.remove< _Components::Disabled>();
							}

							enableChildren(c, enable);
						});
				};

		
			if (!enabled && !selectedEntity.has<_Components::Disabled>())
			{
				selectedEntity.world().defer_begin();
				enableChildren(selectedEntity, enabled);
				selectedEntity.world().defer_end();

			}
			if (enabled && selectedEntity.has<_Components::Disabled>())
			{
				selectedEntity.world().defer_begin();
				enableChildren(selectedEntity, enabled);
				selectedEntity.world().defer_end();
			}
		}

		ImGui::SameLine();


		if (selectedEntity)
		{
			ImGui::Text("Name");
			ImGui::SameLine();
			ImGui::InputText(((std::string)"##" + "Name").c_str(), entityName.data(), entityName.size() + 1,
				ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) -> int
				{
					if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
					{
						auto* str = static_cast<std::string*>(data->UserData);
						str->resize(data->BufTextLen);
						data->Buf = str->data();
					}
					return 0;
				}, &entityName);
			if (ImGui::IsItemActivated())
			{
				saveName = true;
			}
			else if (ImGui::IsItemDeactivated())
			{
				if (!engine->sceneManager->currentScene->_LookupEntity(entityName.data()) && saveName)
				{
					selectedEntity.set_name(entityName.c_str());
					saveName = false;
				}
			}
			else 
			{
				entityName = selectedEntity.name().c_str();
			}
			
			ImGui::Separator();

			engine->sceneManager->currentScene->world.defer_begin();
			selectedEntity.each([&](flecs::id compId) 
			{
				if (compId.is_entity())
				{
					if (engine->sceneManager->currentScene->componentsTable.find(compId.entity().name().c_str()) != engine->sceneManager->currentScene->componentsTable.end())
					{
						DrawComponentElements(compId.entity().name().c_str(), selectedEntity);
						ProjectDrawComponentElements(compId.entity().name().c_str(), selectedEntity);
						ImGui::Separator();
					}
				}
			});
			engine->sceneManager->currentScene->world.defer_end();

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
					if (!comp.first.empty())
					{
						if (ImGui::MenuItem(comp.first.c_str()))
						{
							selectedEntity.add(comp.second);
						}
						ImGui::Separator();
					}
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
				EditorElement::Checkbox(system.second.enable, system.first.c_str());
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
		//Tools bar
		ImGui::Columns(4, 0, false);
		const char* items[] = {"2D", "3D"};
		ImGui::Text("View");
		ImGui::SameLine();
		ImGui::Combo("##View", (int*)&cameraMode, items, IM_ARRAYSIZE(items));
		ImGui::Columns(1);

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
		if (cameraMode == CameraMode::CAMERA2D)
		{

			BeginTextureMode(editorCameraRenderTarget);
			BeginMode2D(editorCamera2D);

			flecs::query cameras = engine->sceneManager->currentScene->world.query<Components::Camera2DComponent, Components::TransformComponent>();
			Components::Camera2DComponent* c2dc = nullptr;
			Components::TransformComponent* tc = nullptr;

			cameras.each([&](flecs::entity e, Components::Camera2DComponent& cc, Components::TransformComponent& _tc)
				{
					if (cc.isMain)
					{
						c2dc = &cc;
						tc = &_tc;
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

			engine->sceneManager->currentScene->renderer.RenderQueued2D();
			engine->sceneManager->currentScene->renderer.RenderUIQueued();


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
					glm::translate(glm::mat4(1.0f), glm::vec3(tc->GetWorldPosition().x + c2dc->camera.offset.x, tc->GetWorldPosition().y + c2dc->camera.offset.y, 0.0f))
					* glm::rotate(glm::mat4(1.0f), -glm::radians(c2dc->camera.rotation), glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::scale(glm::mat4(1.0f), glm::vec3((1.0f / c2dc->camera.zoom), (1.0f / c2dc->camera.zoom), 1.0f))
					* glm::translate(glm::mat4(1.0f), -glm::vec3(tc->GetWorldPosition().x + c2dc->camera.offset.x, tc->GetWorldPosition().y + c2dc->camera.offset.y, 0.0f))
					* glm::translate(glm::mat4(1.0f), glm::vec3(tc->GetWorldPosition().x + c2dc->camera.offset.x, tc->GetWorldPosition().y + c2dc->camera.offset.y, 0.0f));

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
			//Render Canvas Rectangle
			flecs::query UICanvases = engine->sceneManager->currentScene->world.query<Components::UI::UICanvasComponent, Components::TransformComponent>();
			Components::UI::UICanvasComponent* canvas = nullptr;
			tc = nullptr;

			UICanvases.each([&](flecs::entity e, Components::UI::UICanvasComponent& c, Components::TransformComponent& _tc)
				{
					if (c.isMain)
					{
						canvas = &c;
						tc = &_tc;
					}
				});

			//Draw Camrea rectangle
			if (canvas)
			{
				std::vector<glm::vec2> vertices = {
										{0.0f                                        , canvas->canvasRenderTarget.texture.height}, // Bottom-left
										{canvas->canvasRenderTarget.texture.width            , canvas->canvasRenderTarget.texture.height}, // Bottom-right
										{canvas->canvasRenderTarget.texture.width, 0.0f}     , // Top-right
										{0.0f,                                         0.0f}  // Top-left
				};


				std::vector<glm::vec2> transformedVertices = vertices;

				DrawLine((int)transformedVertices[0].x, (int)transformedVertices[0].y, (int)transformedVertices[1].x, (int)transformedVertices[1].y, BROWN);
				DrawLine((int)transformedVertices[1].x, (int)transformedVertices[1].y, (int)transformedVertices[2].x, (int)transformedVertices[2].y, BROWN);
				DrawLine((int)transformedVertices[2].x, (int)transformedVertices[2].y, (int)transformedVertices[3].x, (int)transformedVertices[3].y, BROWN);
				DrawLine((int)transformedVertices[3].x, (int)transformedVertices[3].y, (int)transformedVertices[0].x, (int)transformedVertices[0].y, BROWN);
			}



			EndMode2D();
			EndTextureMode();

		}
		else if (cameraMode == CameraMode::CAMERA3D)
		{
			// render scene.
			BeginTextureMode(editorCameraRenderTarget);
			BeginMode3D(editorCamera3D);

			flecs::query cameras = engine->sceneManager->currentScene->world.query<Components::Camera3DComponent, Components::TransformComponent>();
			Components::Camera3DComponent* c3dc = nullptr;
			Components::TransformComponent* tc = nullptr;

			cameras.each([&](flecs::entity e, Components::Camera3DComponent& cc, Components::TransformComponent& _tc)
				{
					if (cc.isMain)
					{
						c3dc = &cc;
						tc = &_tc;
					}
				});
			if (c3dc)
			{
				ClearBackground(GLMVec4ToRayColor(c3dc->backgroundColor));
			}
			else
			{
				ClearBackground(GLMVec4ToRayColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			}

			if (c3dc)
			{
				if (c3dc->skyboxTexture)
				{
					rlDisableBackfaceCulling();
					rlDisableDepthMask();
					DrawModel(c3dc->skyboxModel, { 0, 0, 0 }, 1.0f, WHITE);
					rlEnableBackfaceCulling();
					rlEnableDepthMask();
				}
			}

			engine->sceneManager->currentScene->renderer.RenderQueued3D();
			DrawGrid(10, 1.0f);


			EndMode3D();
			EndTextureMode();
		}


		rlImGuiImageRenderTextureFit(&editorCameraRenderTarget.texture, false);
		//Draw ImGuizmo stuff here.
		ImGuizmo::BeginFrame();
		usingImGuizmo = false;
		if (selectedEntity)
		{
			Components::TransformComponent* tc = selectedEntity.get_mut<Components::TransformComponent>();
			if (tc) 
			{
				if (cameraMode == CameraMode::CAMERA2D)
				{
					ImGuizmo::SetOrthographic(true);
				}
				else if (cameraMode == CameraMode::CAMERA3D)
				{
					ImGuizmo::SetOrthographic(false);
				}
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(sceneViewportPosition.x, sceneViewportPosition.y, sceneViewportSize.x, sceneViewportSize.y);
				

				glm::mat4 transformMatrix = tc->__worldMatrix;
				Matrix cameraViewMatrix;
				Matrix projectionMatrix;
				if (cameraMode == CameraMode::CAMERA2D)
				{
					cameraViewMatrix = GetCameraMatrix2D(editorCamera2D);
					
					projectionMatrix = MatrixOrtho(0.0f, sceneViewportSize.x, sceneViewportSize.y, 0.0f, 0.1, 100.0f);
				}
				else if (cameraMode == CameraMode::CAMERA3D)
				{
					glm::mat4 lookAt(1.0f);
					glm::vec3 eye = { editorCamera3D.position.x, editorCamera3D.position.y, editorCamera3D.position.z };
					glm::vec3 center = { editorCamera3D.target.x, editorCamera3D.target.y, editorCamera3D.target.z };
					glm::vec3 up = { editorCamera3D.up.x, editorCamera3D.up.y, editorCamera3D.up.z };
					lookAt = glm::lookAt(eye, center, up);
					cameraViewMatrix = GlmMat4ToRaylibMatrix(lookAt);
					
					projectionMatrix = MatrixPerspective(editorCamera3D.fovy * DEG2RAD, sceneViewportSize.x / sceneViewportSize.y, 0.1f, 100.0f);
				}
				ImGuizmo::Manipulate(MatrixToFloat(cameraViewMatrix), MatrixToFloat(projectionMatrix), ImGuizmo::TRANSLATE | ImGuizmo::SCALE | ImGuizmo::ROTATE, ImGuizmo::WORLD, glm::value_ptr(transformMatrix));

				if (ImGuizmo::IsUsing())
				{
					usingImGuizmo = true;
					glm::vec3 skew;
					glm::vec3 pos;
					glm::vec3 scl;
					glm::vec4 pres;
					glm::quat rot;


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

		ImVec2 vec2(0.0f, 0.0f);

		gameViewportFocused = ImGui::IsWindowFocused();

		flecs::query cameras = engine->sceneManager->currentScene->world.query<Components::Camera2DComponent>();
		
		
		ImVec2 area = ImGui::GetContentRegionAvail();

		const Texture mainRenderTexture = engine->sceneManager->currentScene->renderer.mainRenderTarget.texture;

		float scale = area.x / mainRenderTexture.width;

		float y = mainRenderTexture.height * scale;
		if (y > area.y)
		{
			scale = area.y / mainRenderTexture.height;
		}

		int sizeY = int(mainRenderTexture.height * scale);

		vec2 = ImGui::GetCursorScreenPos();
		vec2.y = (area.y / 2 - sizeY / 2) + (vec2.y - .5f);
		gameViewportPosition = *((glm::vec2*)&vec2);

		const Texture* rt = &mainRenderTexture;

		rlImGuiImageRenderTextureFit(&engine->sceneManager->currentScene->renderer.mainRenderTarget.texture, true);

		gameViewportSize = glm::vec2(mainRenderTexture.width * scale, mainRenderTexture.height * scale);

		ImGui::End();
		ImGui::GetStyle().WindowPadding = oldPadding;
	}

	glm::vec2 Editor::SceneViewportMousePos()
	{
		ImVec2 pos = ImGui::GetMousePos();
		glm::vec2 mousePos(pos.x, pos.y);
		mousePos = mousePos - sceneViewportPosition;
		return mousePos;
	}

	void Editor::UpdateEditor(float deltaTime)
	{
		if (isSceneViewHovered)
		{
			if (cameraMode == CameraMode::CAMERA2D)
			{
				glm::vec2 sceneViewportMousePos = SceneViewportMousePos();
				Vector2 mouseWorldPos = GetScreenToWorld2D(Vector2{ sceneViewportMousePos.x, sceneViewportMousePos.y }, editorCamera2D);
				if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
				{
					Vector2 delta = GetMouseDelta();
					delta.x *= -1.0f / editorCamera2D.zoom;
					delta.y *= -1.0f / editorCamera2D.zoom;
					editorCamera2D.target.x = editorCamera2D.target.x + delta.x;
					editorCamera2D.target.y = editorCamera2D.target.y + delta.y;
				}
				float wheel =  GetMouseWheelMove();
				
				if (wheel)
				{
					editorCamera2D.offset = Vector2{ sceneViewportMousePos.x, sceneViewportMousePos.y };
					editorCamera2D.target = mouseWorldPos;
					float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
					if (wheel < 0) scaleFactor = 1.0f / scaleFactor;
					editorCamera2D.zoom = glm::clamp(editorCamera2D.zoom * scaleFactor, 0.005f, 64.0f);
				}
			}
			else if (cameraMode == CameraMode::CAMERA3D) 
			{
				float mouseWheelDelta = GetMouseWheelMove();

				Vector3 forward = Vector3Subtract(editorCamera3D.target, editorCamera3D.position);
				forward = Vector3Normalize(forward);


				editorCamera3D.position = Vector3Add(editorCamera3D.position, Vector3Scale(forward, mouseWheelDelta * editorCameraZoom * deltaTime));
				editorCamera3D.target = Vector3Add(editorCamera3D.target, Vector3Scale(forward, mouseWheelDelta * editorCameraZoom * deltaTime));


				Vector3 right = Vector3CrossProduct(forward, editorCamera3D.up);
				right = Vector3Normalize(right);

				if(ImGui::IsKeyDown(ImGuiKey_W))
				{
					
					editorCamera3D.position = Vector3Add(editorCamera3D.position, Vector3Scale(forward, editorCameraMoveSpeed * deltaTime));
					editorCamera3D.target = Vector3Add(editorCamera3D.target, Vector3Scale(forward, editorCameraMoveSpeed * deltaTime));
				}
				if (ImGui::IsKeyDown(ImGuiKey_S))
				{

					editorCamera3D.position = Vector3Subtract(editorCamera3D.position, Vector3Scale(forward, editorCameraMoveSpeed * deltaTime));
					editorCamera3D.target = Vector3Subtract(editorCamera3D.target, Vector3Scale(forward, editorCameraMoveSpeed * deltaTime));
				}
				if (ImGui::IsKeyDown(ImGuiKey_A))
				{

					editorCamera3D.position = Vector3Subtract(editorCamera3D.position, Vector3Scale(right, editorCameraMoveSpeed * deltaTime));
					editorCamera3D.target = Vector3Subtract(editorCamera3D.target, Vector3Scale(right, editorCameraMoveSpeed * deltaTime));
				}
				if (ImGui::IsKeyDown(ImGuiKey_D))
				{

					editorCamera3D.position = Vector3Add(editorCamera3D.position, Vector3Scale(right, editorCameraMoveSpeed * deltaTime));
					editorCamera3D.target = Vector3Add(editorCamera3D.target, Vector3Scale(right, editorCameraMoveSpeed * deltaTime));
				}

				if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
				{
					Vector2 mouseDelta = GetMouseDelta();

					editorCamera3D.position = Vector3Subtract(editorCamera3D.position, Vector3Scale(right, mouseDelta.x  * deltaTime));
					editorCamera3D.target = Vector3Subtract(editorCamera3D.target, Vector3Scale(right, mouseDelta.x * deltaTime));
				}

				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
				{
					Vector2 mouseDelta = GetMouseDelta();
					float sensitivity = editorCameraSensitiviy;

					float yaw = mouseDelta.x * sensitivity;
					float pitch = -mouseDelta.y * sensitivity;

					Matrix rotationYaw = MatrixRotate(editorCamera3D.up, -yaw);
					forward = Vector3Transform(forward, rotationYaw);

					Vector3 rightAxis = Vector3CrossProduct(forward, editorCamera3D.up);
					rightAxis = Vector3Normalize(rightAxis);

					Matrix rotationPitch = MatrixRotate(rightAxis, pitch);
					forward = Vector3Transform(forward, rotationPitch);

					editorCamera3D.target = Vector3Add(editorCamera3D.position, forward);
				}
			}
		}

		if (isSceneViewHovered)
		{
			if (ImGui::IsMouseClicked(MOUSE_BUTTON_LEFT))
			{

				if (glm::vec2(colorPickingBuffer.texture.width, colorPickingBuffer.texture.height) != sceneViewportSize)
				{
					UnloadRenderTexture(colorPickingBuffer);
					colorPickingBuffer = LoadRenderTexture((int)sceneViewportSize.x, (int)sceneViewportSize.y, PIXELFORMAT_UNCOMPRESSED_R32);
					SetTextureFilter(colorPickingBuffer.texture, TEXTURE_FILTER_POINT);
				}

				if (cameraMode == CameraMode::CAMERA2D)
				{
					BeginTextureMode(colorPickingBuffer);
					BeginMode2D(editorCamera2D);
					ClearBackground(BLANK);
					BeginShaderMode(colorPickingShader);
					int idUniformLoc = GetShaderLocation(colorPickingShader, "id");
					EndShaderMode();
					//Draw Scene
					for (const auto& tex2d : engine->sceneManager->currentScene->renderer.texture2DRenderQueue)
					{
						if (tex2d.entity)
						{
							BeginShaderMode(colorPickingShader);
							float id = (float)((int)tex2d.entity);
							SetShaderValue(colorPickingShader, idUniformLoc, (const void*)&id, SHADER_UNIFORM_FLOAT);
							DrawTexturePro(tex2d.texture, tex2d.source, tex2d.dest, tex2d.origin, tex2d.rotation, tex2d.tint);
							EndShaderMode();
						}
					}
					//Draw UI
					for (const auto& tex2d : engine->sceneManager->currentScene->renderer.UIRenderQueue)
					{
						if (tex2d.entity)
						{
							BeginShaderMode(colorPickingShader);
							float id = (float)((int)tex2d.entity);
							SetShaderValue(colorPickingShader, idUniformLoc, (const void*)&id, SHADER_UNIFORM_FLOAT);
							DrawTexturePro(tex2d.texture, tex2d.source, tex2d.dest, tex2d.origin, tex2d.rotation, tex2d.tint);
							EndShaderMode();
						}
					}

					EndMode2D();
					EndTextureMode();
				}
				else if (cameraMode == CameraMode::CAMERA3D)
				{
					BeginTextureMode(colorPickingBuffer);
					BeginMode3D(editorCamera3D);
					ClearBackground(BLANK);
					int idUniformLoc = GetShaderLocation(colorPickingShader, "id");
					//Draw Scene
					for (const auto& m3d : engine->sceneManager->currentScene->renderer.model3DRenderQueue)
					{
						if (m3d.entity)
						{
							std::vector<Shader> oldShaders;
							for (size_t i = 0; i < m3d.model.materialCount; i++)
							{
								oldShaders.push_back(m3d.model.materials[i].shader);
								m3d.model.materials[i].shader = colorPickingShader;
							}
						
							float id = (float)((int)m3d.entity);
							SetShaderValue(colorPickingShader, idUniformLoc, (const void*)&id, SHADER_UNIFORM_FLOAT);
							DrawModel(m3d.model, {}, 1.0f, WHITE);

							for (size_t i = 0; i < m3d.model.materialCount; i++)
							{
								m3d.model.materials[i].shader = oldShaders[i];
							}
						}
					}
					EndMode3D();
					EndTextureMode();

				}
				Texture t = colorPickingBuffer.texture;

				Image pickImage = LoadImageFromTexture(t);
				float* pickingData = (float*)pickImage.data;
				glm::vec2 mousePos = SceneViewportMousePos();
				int pixelIndex = (int)(pickImage.height - mousePos.y) * pickImage.width + (int)mousePos.x;
				float id = pickingData[pixelIndex < pickImage.width * pickImage.height ? pixelIndex : 0];
				TraceLog(LOG_DEBUG, "X: %f, Y: %f, ID: %f", mousePos.x, mousePos.y, id);
				if (usingImGuizmo)
				{
					//do nothing brother.	
				}
				else if ((int)id)
				{
					selectedEntity = engine->sceneManager->currentScene->_LookupEntity((flecs::entity)(int)id);
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
			ImColor color = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
			if (log.find("[INFO]") != std::string::npos)
			{
				color = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
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


#endif