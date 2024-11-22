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


	}

	void SceneManager::UnloadScene()
	{
		currentScene = nullptr;
		AssetsManager::GetSingleton()->Clear();
	}
	void SceneManager::SaveScene()
	{

	}


	void SceneManager::SaveSceneAs()
	{

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