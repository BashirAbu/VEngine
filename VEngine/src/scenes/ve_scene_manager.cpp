#include "ve_scene_manager.h"
#include "ve_engine.h"
#include <fstream>
#include <raylib.h>
#include <nlohmann/json.hpp>
#include "platform/ve_platform.h"
#include "utils/ve_serialization.h"
#include "utils/ve_utils.h"
#include <iostream>
#include "ve_assets_manager.h"
#include "ve_engine.h"
#include "components/ve_components.h"
#include "ve_assets_manager.h"
namespace VE
{
	SceneManager::SceneManager() : currentScene(nullptr), mode(SceneMode::Editor)
	{
	}
	SceneManager::~SceneManager()
	{
		delete currentScene;
	}
	void SceneManager::LoadScene(std::filesystem::path scenePath)
	{

		if (currentScene)
		{
			UnloadScene();
		}

		if (scenePath.empty())
		{
			return;
		}

		currentScene = new Scene(SceneType::Scene2D);
		currentScene->scenePath = scenePath;
		nlohmann::ordered_json sceneJson;

#ifdef VE_EDITOR
		std::fstream sceneFile(GetFullPath(scenePath));
		sceneFile >> sceneJson;
#else
		std::string sceneRawJson = VE::AssetsManager::GetSingleton()->LoadScene(scenePath);
		sceneJson = nlohmann::ordered_json::parse(sceneRawJson);
#endif

		nlohmann::ordered_json sceneSystemsJson = sceneJson["scene_systems"];
		
		std::vector<std::string> enabledSystems;

		for (auto systemSceneJson : sceneSystemsJson)
		{
			std::string systemName = (std::string)systemSceneJson;
			enabledSystems.push_back(systemName);
		}

		for (auto enableSystem : enabledSystems)
		{
			if (currentScene->systemsTable[enableSystem].entity)
			{
				currentScene->systemsTable[enableSystem].enable = true;
				flecs::system* s = (flecs::system*)&currentScene->systemsTable[enableSystem].entity;
				s->enable();
			}
		}
					
		std::stringstream ss;
		ss << sceneJson["flecs_world"];
		std::string flecsWorld = ss.str();
		currentScene->world.from_json(flecsWorld.c_str());
	}

	void SceneManager::UnloadScene()
	{
		if (currentScene)
		{
			delete currentScene;
		}
		currentScene = nullptr;
		AssetsManager::GetSingleton()->Clear();
	}
	void SceneManager::SaveScene()
	{
		if (currentScene->scenePath.empty())
		{
			SaveSceneAs();
			return;
		}

		nlohmann::ordered_json sceneJson;

		nlohmann::ordered_json flecsWorldJson = nlohmann::ordered_json::parse(currentScene->SerializeScene());
		sceneJson["flecs_world"] = flecsWorldJson;
		size_t index = 0;
		for (auto system : currentScene->systemsTable)
		{
			if (system.second.enable)
			{
				std::string systemName = "system" + std::to_string(index);
				sceneJson["scene_systems"][systemName] = system.first;
				index++;
			}
		}

		std::stringstream ss;
		ss << sceneJson;
		std::ofstream sceneFile(GetFullPath(currentScene->scenePath));
		sceneFile << sceneJson;
		sceneFile.close();
	}

	void SceneManager::ReloadCurrentScene()
	{
		reloadCurrentScene = true;
	}
	void SceneManager::ChangeScene(std::filesystem::path filepath) 
	{
		changeScenePath = filepath;
		changeScene = true;
	}

	void SceneManager::SaveSceneAs()
	{
#ifdef VE_EDITOR
		currentScene->scenePath = VE::SaveFileDialog(VE_SCENE_FILE_EXTENSION);
#endif
		if (currentScene->scenePath.empty())
			return;
		if (currentScene->scenePath.extension().string() != ("." + (std::string)VE_SCENE_FILE_EXTENSION))
		{
			TraceLog(LOG_ERROR, "Scene files must have %s extension!", VE_SCENE_FILE_EXTENSION);
			return;
		}
		currentScene->scenePath = GetRelativePath(currentScene->scenePath);
		SaveScene();
	}
	void SceneManager::RunCurrentScene()
	{
		if (currentScene)
		{
#ifdef VE_EDITOR
			if (mode == SceneMode::Game)
			{
#endif
				if (!currentScene->started)
				{
					currentScene->Start();
					currentScene->started = true;
				}
				currentScene->Update();
#ifdef VE_EDITOR
			}
#endif
			currentScene->Render();
		}
	}
}