#include "ve_engine.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "platform/ve_platform.h"
#include "ve_assets_manager.h"
#include <fstream>
#include <iostream>
#include "entities/ve_camera_2d_entity.h"
#include "entities/ve_empty_entity.h"

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
			sceneManager->CreateEmptyScene(SceneType::Scene2D);
		}
		else 
		{
			std::string scenePath = engineDesc.projectDetails.path.parent_path().string();
			scenePath += "/" + engineDesc.projectDetails.mainScenePath.string();
			sceneManager->LoadScene(scenePath);
		}

		
		
		//Register builtin entities.
		RegisterEntity(VE_STRINGIFY(EmptyEntity));
		RegisterEntity(VE_STRINGIFY(Camera2DEntity));
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
			ClearBackground(BLACK);
			sceneManager->RunCurrentScene();
			editor->DrawUI();
			EndDrawing();
		}
	}
	void Engine::RegisterEntity(std::string entity)
	{
		for (std::string name : entitiesRegistry) 
		{
			if (name == entity)
			{
				return;
			}
		}
		entitiesRegistry.push_back(entity);
	}
	Entity* Engine::CreateBuiltinEntity(std::string entity)
	{

		Entity* ent = nullptr;
		if (entity == "Camera2DEntity")
		{
			ent = new Camera2DEntity(entity);
			ent->internalName = entity;
		}
		else if (entity == "EmptyEntity")
		{
			ent = new EmptyEntity(entity);
			ent->internalName = entity;
		}
		return ent;
	}
	void Engine::LoadProjectSharedLibrary()
	{
		std::ifstream srcSharedLib(desc.projectDetails.path.parent_path().string() + "/bin/" + "project" + ".dll", std::ios::binary);
		std::ofstream destSharedLib(desc.projectDetails.path.parent_path().string() + "/bin/" + "project" + "_temp.dll", std::ios::binary);

		destSharedLib << srcSharedLib.rdbuf();

		srcSharedLib.close();
		destSharedLib.close();
		projectSharedLibrary->Load(desc.projectDetails.path.parent_path().string() + "/bin/" + "project" + "_temp");

		CreateProjectEntity = (PFN_CreateProjectEntity)projectSharedLibrary->GetProcAddress("CreateProjectEntity");
		VE_ASSERT(CreateProjectEntity);
		OnSharedLibraryEntry = (PFN_OnSharedLibraryEntry)projectSharedLibrary->GetProcAddress("OnSharedLibraryEntry");
		VE_ASSERT(OnSharedLibraryEntry);
		ptr = (get_ptr)projectSharedLibrary->GetProcAddress("get_ptr");
		OnSharedLibraryEntry();
	}
	void Engine::ReloadProjectSharedLibrary()
	{
		UnloadProjectSharedLibrary();
		LoadProjectSharedLibrary();
	}
	void Engine::UnloadProjectSharedLibrary()
	{
		projectSharedLibrary->Unload();
		CreateProjectEntity = nullptr;
		OnSharedLibraryEntry = nullptr;
	}

}