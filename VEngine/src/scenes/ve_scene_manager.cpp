#include "ve_scene_manager.h"
#include "ve_engine.h"
#include <fstream>
#include <raylib.h>
#include <nlohmann/json.hpp>
#include "platform/ve_platform.h"
#include "utils/ve_serialization.h"
#include <iostream>
#include "ve_assets_manager.h"
#include "entities/ve_entity.h"
#include "ve_scene_2d.h"
namespace VE 
{
	SceneManager::SceneManager() : currentScene(nullptr), mode(SceneMode::Editor), selectedEntity(nullptr)
	{
	}
	SceneManager::~SceneManager()
	{
		delete currentScene;
	}
	void SceneManager::LoadScene(std::filesystem::path scenePath)
	{
		std::ifstream sceneFile(scenePath);
		nlohmann::json sceneJson = nlohmann::json::parse(sceneFile);

		if (currentScene) 
		{
			UnloadScene();
			currentScene = nullptr;
		}
		
		if (sceneJson["type"] == "2D")
		{
			currentScene = new Scene2D();
		}
		else 
		{
			//handle 3d scene here
		}


		currentScene->name = sceneJson["name"];
		currentScene->scenePath = scenePath;
		if (SceneType::Scene2D == currentScene->GetSceneType())
		{
			Scene2D* s2d = (Scene2D*)currentScene;
			s2d->clearColor = Deserialze::Vec4(sceneJson["clear_color"]);
		}
		if (sceneJson.contains("entities"))
		{
			for (nlohmann::json entityJson : sceneJson["entities"])
			{
				std::string entName = entityJson["name"];
				Entity* entity = CreateProjectEntity(entName);
				if (!entity)
				{
					entity = Engine::GetSingleton()->CreateBuiltinEntity(entName);
				}
				entity->Deserialize(entityJson);
				entity->ComponentsDeserialize(entityJson["components"]);
				//currentScene->entities.push_back(entity);
			}
		}

	}
	void SceneManager::UnloadScene()
	{
		if(currentScene)
			delete currentScene;
		currentScene = nullptr;
		selectedEntity = nullptr;
		AssetsManager::GetSingleton()->Clear();
	}
	void SceneManager::SaveScene()
	{
		if (currentScene->scenePath.empty()) 
		{
			SaveSceneAs();
			return;
		}

		//savescene.
		nlohmann::json sceneJson;
		currentScene->name = currentScene->scenePath.stem().string();
		sceneJson["name"] = currentScene->name;
		if (SceneType::Scene2D == currentScene->GetSceneType())
		{
			Scene2D* s2d = (Scene2D*)currentScene;
			sceneJson["clear_color"] = Serialize::Vec4(s2d->clearColor);
			sceneJson["type"] = "2D";
		}

		//serialize entities.
		size_t index = 0;
		for (Entity* entity : currentScene->entities)
		{
			if (entity->parent)
			{
				continue;
			}
			nlohmann::json entityJson;
			entityJson["name"] = entity->name;
			entity->Serialize(entityJson);
			entity->ComponentsSerialize(entityJson["components"]);
			std::string genName = "entity" + std::to_string(index);
			sceneJson["entities"][genName] = entityJson;
			index++;
		}

		std::ofstream sceneFile(currentScene->scenePath);
		sceneFile << sceneJson;
		sceneFile.close();
	}
	Scene* SceneManager::CreateEmptyScene(SceneType type)
	{
		Scene* scene = nullptr;
		if (currentScene)
		{
			UnloadScene();
		}
		if (type == SceneType::Scene2D)
		{
			scene = new Scene2D();
		}
		else 
		{
			//3d scene stuff
		}
		currentScene = scene;
		return scene;
	}
	void SceneManager::SaveSceneAs()
	{
		//show file dialog.
		currentScene->scenePath = VE::SaveFileDialog(VE_SCENE_FILE_EXTENSION);
		if (currentScene->scenePath.empty())
			return;
		if (currentScene->scenePath.extension().string() != ("." + (std::string)VE_SCENE_FILE_EXTENSION))
		{
			TraceLog(LOG_ERROR, "Scene files must have %s extension!", VE_SCENE_FILE_EXTENSION);
			return;
		}
		TraceLog(LOG_INFO, "%s", currentScene->scenePath.string().c_str());
		//serialize the scene.
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
				currentScene->Update(GetFrameTime());
			}
			
			currentScene->Render();
		}
	}
}