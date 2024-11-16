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
#include "entities/ve_empty_entity.h"
#include "ve_engine.h"
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
		if (scenePath.empty()) 
		{
			return;
		}
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
				bool isConstruct = entityJson.contains("construct")? (bool)entityJson["construct"] : false;
				if (isConstruct)
				{
					Entity* construct = LoadConstruct(entityJson["construct_path"]);
					construct->Deserialize(entityJson);
					construct->ComponentsDeserialize(entityJson["components"]);
				}
				else 
				{
					DeserializeChildren(entityJson);
				}
			}
		}

	}
	Entity* SceneManager::DeserializeChildren(nlohmann::json entityJson)
	{
		std::string entName = entityJson["internal_name"];
		Entity* entity = CreateProjectEntity(entName);
		if (!entity)
		{
			entity = Engine::GetSingleton()->CreateBuiltinEntity(entName);
		}
		entity->name = entityJson.contains("name")? entityJson["name"] : "";
		entity->Deserialize(entityJson);
		entity->ComponentsDeserialize(entityJson["components"]);
		for (nlohmann::json childJson : entityJson["children"])
		{
			Entity* child = DeserializeChildren(childJson);
			entity->AddChild(child);
		}
		return entity;
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
			if (!entity->construct)
			{
				entityJson = SerializeChildren(entity, index);
			}
			else
			{
				entityJson["construct"] = entity->construct;
				entityJson["internal_name"] = entity->internalName;
				entityJson["name"] = entity->name;
				entityJson["construct_path"] =  entity->constructPath.generic_string();
				entity->Serialize(entityJson);
				entity->ComponentsSerialize(entityJson["components"]);
			}
			std::string genName = "entity" + std::to_string(index);
			sceneJson["entities"][genName] = entityJson;
			index++;
		}

		std::ofstream sceneFile(currentScene->scenePath);
		sceneFile << sceneJson;
		sceneFile.close();
	}
	nlohmann::json SceneManager::SerializeChildren(Entity* entity, size_t& index)
	{

		nlohmann::json entityJson;
		entityJson["internal_name"] = entity->internalName;
		entityJson["name"] = entity->name;
		entityJson["construct"] = entity->construct;
		entity->Serialize(entityJson);
		entity->ComponentsSerialize(entityJson["components"]);
		
		for (auto child : entity->children)
		{
			
			std::string childGenName = "entity" + std::to_string(index);
			entityJson["children"][childGenName] = SerializeChildren(child, index);
			index++;
		}
		return entityJson;
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
	Entity* SceneManager::LoadConstruct(std::filesystem::path constructPath)
	{
		if (constructPath.empty())
		{
			return nullptr;
		}
		std::filesystem::path relativePath = constructPath.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path());
		std::filesystem::path fullPath = Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().string() + "/" + (!relativePath.empty()? relativePath.string() : constructPath.string());
		std::fstream constructFile(fullPath);
		nlohmann::json constructJson = nlohmann::json::parse(constructFile);
		constructFile.close();
		constructJson["name"];
		Entity* construct = new EmptyEntity(constructJson["name"]);
		construct->SetInternalName(constructJson["name"]);


		if (constructJson.contains("entities"))
		{
			for (nlohmann::json entityJson : constructJson["entities"])
			{
				construct->AddChild(DeserializeChildren(entityJson));
			}
		}
		construct->construct = true;
		construct->constructPath = (!relativePath.empty() ? relativePath: constructPath).generic_string();
		return construct;
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

			//Destroy entites.
			for (auto itr = currentScene->entities.begin(); itr != currentScene->entities.end();)
			{
				if ((*itr)->destroy)
				{
					delete* itr;
					itr = currentScene->entities.erase(itr);
				}
				else 
				{
					itr++;
				}
			}
		}
	}
}