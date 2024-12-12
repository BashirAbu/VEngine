#include "ve_engine.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "platform/ve_platform.h"
#include "ve_assets_manager.h"
#include <fstream>
#include <iostream>
#include "platform/ve_flecs_os_backend.h"
#include "ShapingEngine.hpp"
#include "utils/ve_utils.h"



namespace ShapingEngine
{
	std::map<int, Glyph> glyphs;
}

namespace VE 
{
	void CustomLogCallback(int logLevel, const char* text, va_list args) {

		std::string msg;
		
		va_list argsCopy;
		va_copy(argsCopy, args);
		int size = vsnprintf(nullptr, 0, text, argsCopy);
		va_end(argsCopy);

		if (size <= 0) 
		{
			msg = "";
		}

		std::vector<char> buffer(size + 1);
		vsnprintf(buffer.data(), buffer.size(), text, args);
		msg = buffer.data();
		msg = msg + '\n';
		switch (logLevel)
		{
		case LOG_INFO: msg = "[INFO]: " + msg; break;
		case LOG_ERROR:  msg = "[ERROR]: " + msg; break;
		case LOG_WARNING:  msg = "[WARN]: " + msg; break;
		case LOG_DEBUG:  msg = "[DEBUG]: " + msg; break;
		case LOG_FATAL:  msg = "[FATAL]: " + msg; break;
		default: break;
		}
#ifdef VE_EDITOR
		AddLog(msg);
#endif
		printf("%s", msg.c_str());
		buffer.clear();
	}
	Engine* Engine::singleton = nullptr;
	
	Engine::Engine(EngineDesc engineDesc)
	{

		stdcpp_set_os_api();
		singleton = this;
		//Parse project file.
		nlohmann::json projectJson;
#ifdef VE_EDITOR
		std::ifstream projectFile(engineDesc.projectDetails.path);
		projectJson = nlohmann::json::parse(projectFile);
#else
		std::string rawJson = VE::AssetsManager::GetSingleton()->LoadProject(engineDesc.projectDetails.path);
		projectJson = nlohmann::json::parse(rawJson);
#endif
		engineDesc.projectDetails.name = projectJson["project_name"];
		engineDesc.projectDetails.title = projectJson["title"];
		engineDesc.projectDetails.width = projectJson["width"];
		engineDesc.projectDetails.height = projectJson["height"];
		engineDesc.projectDetails.renderWidth = projectJson["render_width"];
		engineDesc.projectDetails.renderHeight = projectJson["render_height"];
		engineDesc.projectDetails.vsync = projectJson["vsync"];
		engineDesc.projectDetails.assetsPath = engineDesc.projectDetails.path.parent_path().string() + "/assets/";
		engineDesc.projectDetails.mainScenePath = (std::string)projectJson["main_scene_path"];
		desc = engineDesc;
		
		uint32_t configFlags = 0;
		if (engineDesc.projectDetails.vsync)
		{
			configFlags |= FLAG_VSYNC_HINT;
		}
		SetTraceLogLevel(LOG_DEBUG);
		SetTraceLogCallback(CustomLogCallback);
		InitAudioDevice();
		SetConfigFlags(configFlags);
		InitWindow(engineDesc.projectDetails.width, engineDesc.projectDetails.height, engineDesc.projectDetails.title.c_str());
		SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
		
#ifdef VE_EDITOR
		editor = new Editor(this);
		projectSharedLibrary = new SharedLibrary();
#else
#ifdef VE_WINDOWS
		Image icon = LoadImage("icon.png");
		SetWindowIcon(icon);
		UnloadImage(icon);
#endif
#endif


		sceneManager = new SceneManager();
		
		if (engineDesc.projectDetails.mainScenePath.empty())
		{
			//Create emtpy scene;
			sceneManager->currentScene = new Scene(SceneType::Scene2D);
		}
		else 
		{
			sceneManager->LoadScene(engineDesc.projectDetails.mainScenePath);
		}

	}

	Engine::~Engine()
	{
		ShapingEngine::glyphs.clear();
#ifdef VE_EDITOR
		delete editor;
#endif
		delete sceneManager;
#ifdef VE_EDITOR
		delete projectSharedLibrary;
#endif
		delete AssetsManager::GetSingleton();
		CloseAudioDevice();
		CloseWindow();
#ifdef VE_EDITOR
		logs->clear();
		delete logs;
#endif
	}
	Engine* Engine::GetSingleton()
	{
		VE_ASSERT(singleton);
		return singleton;
	}

	void Engine::Run()
	{
#ifndef __EMSCRIPTEN__
		while (!WindowShouldClose())
		{
#endif

			BeginDrawing();
			ClearBackground(BLANK);
			sceneManager->RunCurrentScene();
#ifdef VE_EDITOR
			editor->DrawUI();
#else
			//Draw main render target directly to the window.
			RenderTexture mainTarget = sceneManager->currentScene->GetMainRenderTarget();
			RaylibDrawTexturTargeteLetterBox(mainTarget, glm::vec2((float)GetScreenWidth(), (float)GetScreenHeight()));
#endif

			EndDrawing();

			if (sceneManager->reloadCurrentScene)
			{
				sceneManager->LoadScene(sceneManager->currentScene->GetScenePath());
				sceneManager->reloadCurrentScene = false;
				sceneManager->changeScene = false;
#ifdef VE_EDITOR

				editor->selectedEntity = flecs::entity();
#endif

			}

			if (sceneManager->changeScene)
			{
				sceneManager->LoadScene(sceneManager->changeScenePath);
				sceneManager->reloadCurrentScene = false;
				sceneManager->changeScene = false;
#ifdef VE_EDITOR

				editor->selectedEntity = flecs::entity();
#endif

			}

#ifndef __EMSCRIPTEN__
		}
#endif

	}
	
#ifdef VE_EDITOR
	void Engine::LoadProjectSharedLibrary()
	{
		std::ifstream srcSharedLib(desc.projectDetails.path.parent_path().string() + "/bin/" + "project" + ".dll", std::ios::binary);
		std::ofstream destSharedLib(desc.projectDetails.path.parent_path().string() + "/bin/" + "project" + "_temp.dll", std::ios::binary);

		destSharedLib << srcSharedLib.rdbuf();

		srcSharedLib.close();
		destSharedLib.close();
		projectSharedLibrary->Load(desc.projectDetails.path.parent_path().string() + "/bin/" + "project" + "_temp");

		OnSharedLibraryEntry = (PFN_OnSharedLibraryEntry)projectSharedLibrary->GetProcAddress("OnSharedLibraryEntry");
		VE_ASSERT(OnSharedLibraryEntry);
		ProjectDrawComponentElements = (PFN_ProjectDrawComponentElements)projectSharedLibrary->GetProcAddress("ProjectDrawComponentElements");
		VE_ASSERT(ProjectDrawComponentElements);
	}
	void Engine::ReloadProjectSharedLibrary()
	{
		UnloadProjectSharedLibrary();
		LoadProjectSharedLibrary();
	}
	void Engine::UnloadProjectSharedLibrary()
	{
		projectSharedLibrary->Unload();
		OnSharedLibraryEntry = nullptr;
	}
#endif

}