#include "ve_engine.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "platform/ve_platform.h"
#include "ve_assets_manager.h"
#include <fstream>
#include <iostream>

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
		AddLog(msg);
		printf(msg.c_str());
		buffer.clear();
	}
	Engine* Engine::singleton = nullptr;
	Engine::Engine(EngineDesc engineDesc)
	{
		singleton = this;
		//Parse project file.
		std::ifstream projectFile(engineDesc.projectDetails.path);
		nlohmann::json projectJson = nlohmann::json::parse(projectFile);
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
		
		
		editor = new Editor(this);


		projectSharedLibrary = new SharedLibrary();

		LoadProjectSharedLibrary();

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
	typedef VE_PROJECT_API int* (*get_ptr)();
	get_ptr ptr;
	Engine::~Engine()
	{
		delete editor;
		delete sceneManager;
		delete projectSharedLibrary;
		delete AssetsManager::GetSingleton();
		CloseAudioDevice();
		CloseWindow();
		logs->clear();
		delete logs;
	}
	Engine* Engine::GetSingleton()
	{
		VE_ASSERT(singleton);
		return singleton;
	}

	void Engine::Run()
	{
		while (!WindowShouldClose())
		{
			BeginDrawing();
			ClearBackground(BLANK);
			sceneManager->RunCurrentScene();
			editor->DrawUI();
			EndDrawing();
		}
	}
	
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

}