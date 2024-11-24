#include "ve_scene_manager.h"
#include "ve_engine.h"
#include <fstream>
#include <raylib.h>
#include <nlohmann/json.hpp>
#include "platform/ve_platform.h"
#include "utils/ve_serialization.h"
#include <iostream>
#include "ve_assets_manager.h"
#include "ve_engine.h"
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
		std::filesystem::path fullpath = Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().string() + "/assets/" + scenePath.string();
		currentScene = new Scene(SceneType::Scene2D);
		currentScene->scenePath = scenePath;
		std::fstream sceneFile(fullpath);
		std::string sceneJson;
		std::stringstream ss;
		ss << sceneFile.rdbuf();
		sceneJson = ss.str();

		currentScene->world.from_json(sceneJson.c_str());
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

		std::string sceneJson =  currentScene->world.to_json().c_str();
		std::ofstream sceneFile(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().string() + "/assets/" + currentScene->scenePath.string());
		sceneFile << sceneJson;
		sceneFile.close();
	}


	void SceneManager::SaveSceneAs()
	{
		currentScene->scenePath = VE::SaveFileDialog(VE_SCENE_FILE_EXTENSION);
		if (currentScene->scenePath.empty())
			return;
		if (currentScene->scenePath.extension().string() != ("." + (std::string)VE_SCENE_FILE_EXTENSION))
		{
			TraceLog(LOG_ERROR, "Scene files must have %s extension!", VE_SCENE_FILE_EXTENSION);
			return;
		}
		std::filesystem::path relativePath = currentScene->scenePath.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().generic_string() + "/assets");
		currentScene->scenePath = relativePath;
		SaveScene();
	}
	void SceneManager::RunCurrentScene()
	{
		if (currentScene)
		{
			if (mode == SceneMode::Game)
			{
				if (!currentScene->started)
				{
					currentScene->Start();
					currentScene->started = true;
				}
				currentScene->Update();
			}
			currentScene->Render();
		}
	}
}