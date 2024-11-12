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
		engineDesc.projectDetails.mainScenePath = (std::string)projectJson["main_scene_path"];
		engineDesc.projectDetails.vsync = projectJson["vsync"];
		engineDesc.projectDetails.assetsPath = engineDesc.projectDetails.path.parent_path().string() + "/assets/";
		desc = engineDesc;
		
		uint32_t configFlags = 0;
		if (engineDesc.projectDetails.vsync)
		{
			configFlags |= FLAG_VSYNC_HINT;
		}
		SetConfigFlags(configFlags);
		InitWindow(engineDesc.projectDetails.width, engineDesc.projectDetails.height, engineDesc.projectDetails.title.c_str());
		SetWindowState(FLAG_WINDOW_RESIZABLE);

		projectSharedLibrary = new SharedLibrary();

		LoadProjectSharedLibrary();

		std::string scenePath = engineDesc.projectDetails.path.parent_path().string();
		scenePath += "/" + engineDesc.projectDetails.mainScenePath.string();
		sceneManager = new SceneManager();
		sceneManager->LoadScene(scenePath);

		editor = new Editor(this);
		
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
			editor->UpdateEditor(GetFrameTime());
			sceneManager->RunCurrentScene();
			editor->DrawUI();
			EndDrawing();
		}
	}
	void Engine::RegisterEntity(std::string entityName)
	{
		for (std::string name : entitiesRegistry) 
		{
			if (name == entityName)
			{
				return;
			}
		}
		entitiesRegistry.push_back(entityName);
	}
	Entity* Engine::CreateBuiltinEntity(std::string entityName)
	{
		if (entityName == "Camera2DEntity")
		{
			return new Camera2DEntity(VE_STRINGIFY(Camera2DEntity));
		}
		else if (entityName == "EmptyEntity") 
		{
			return new EmptyEntity(VE_STRINGIFY(EmptyEntity));
		}
		return nullptr;
	}
	void Engine::LoadProjectSharedLibrary()
	{
		std::ifstream srcSharedLib(desc.projectDetails.path.parent_path().string() + "/bin/" + desc.projectDetails.name + ".dll", std::ios::binary);
		std::ofstream destSharedLib(desc.projectDetails.path.parent_path().string() + "/bin/" + desc.projectDetails.name + "_temp.dll", std::ios::binary);

		destSharedLib << srcSharedLib.rdbuf();

		srcSharedLib.close();
		destSharedLib.close();
		projectSharedLibrary->Load(desc.projectDetails.path.parent_path().string() + "/bin/" + desc.projectDetails.name + "_temp");

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