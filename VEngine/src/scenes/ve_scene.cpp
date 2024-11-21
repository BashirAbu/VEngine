#include "ve_scene.h"
#include "entities/ve_entity.h"
#include "entities/ve_camera_2d_entity.h"
#include "entities/ve_camera_entity.h"
#include "ve_engine.h"
#include "editor/ve_editor_input.h"
#include <imgui.h>


namespace VE 
{
	Scene::Scene(SceneType type)
	{
		sceneType = type;
	}
	void Scene::DeleteEntityChildren(Entity* entity)
	{
		for (auto itr = entity->children.begin(); itr != entity->children.end(); itr++)
		{
			if ((*itr)->children.size() > 0)
			{
				DeleteEntityChildren(*itr);
				
				(*itr)->destroy = true;
				
			}
			else 
			{
				(*itr)->destroy = true;
			}
		}
	}
	Scene::~Scene()
	{
		mainCamera = nullptr;

		for (auto itr = entities.begin(); itr != entities.end();)
		{
		
			delete *itr;
			itr = entities.erase(itr);
			
		}
		entities.clear();
	}
	void Scene::Start()
	{
		for (Entity* entity : entities) 
		{
			entity->Start();
			entity->ComponentsStart();
			entity->started = true;
		}
	}
	void Scene::Update(float deltaTime)
	{
		for (Entity* entity : entities)
		{
			if (!entity->started)
			{
				entity->started = true;
				entity->Start();
			}
		}
		for (Entity* entity : entities)
		{
			if (entity->started)
			{
				entity->PreUpdate(deltaTime);
				entity->ComponentsPreUpdate(deltaTime);
			}
		}

		for (Entity* entity : entities)
		{
			if (entity->started)
			{
				entity->Update(deltaTime);
				entity->ComponentsUpdate(deltaTime);
			}
		}
	}
	void Scene::Render()
	{
		//
		// sort entities based on z
		// 
		entities.sort([](const Entity* a, const Entity* b)
			{
				return a->transformComponent->GetPosition().z < b->transformComponent->GetPosition().z;
			});
		//for each camera, render the scene once.
		for (CameraEntity* camera : cameras)
		{
			BeginTextureMode(*camera->GetRenderTarget());
			if (sceneType == SceneType::Scene2D)
			{
				Camera2DEntity* c2d = (Camera2DEntity*)camera;
				BeginMode2D(c2d->camera2D);
			}
			else 
			{
				//handle 3d.
			}

			Color cc = { (uint8_t)(clearColor.r * 255), (uint8_t)(clearColor.g * 255) , (uint8_t)(clearColor.b * 255) , (uint8_t)(clearColor.a * 255) };
			ClearBackground(cc);
			for (Entity* entity : entities)
			{
				entity->Render();
				entity->ComponentsRender();
			}
			if (sceneType == SceneType::Scene2D)
			{
				EndMode2D();
			}
			else 
			{
				//handle 3d.
			}
			EndTextureMode();
		}
	}
	void Scene::DrawEditorUI()
	{
		for (Entity* entity : entities)
		{
			entity->DrawEditorUI();
			entity->ComponentDrawEditorUI();
		}
	}
	Entity* Scene::GetEntityByName(std::string name)
	{
		for (Entity* ent : entities)
		{
			if (ent->name == name)
			{
				return ent;
			}
		}

		return nullptr;
	}
	void Scene::AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	}
	void Scene::RemoveEntity(Entity* entity)
	{
		for (auto itr = entities.begin(); itr != entities.end(); itr++) 
		{
			if (*itr == entity) 
			{
				delete *itr;
				itr = entities.erase(itr);
				break;
			}
		}
	}

	void Scene::SetMainCamera(CameraEntity* camera)
	{
		for (auto itr = cameras.begin(); itr != cameras.end(); itr++)
		{
			(*itr)->mainCamera = false;
		}
		for (auto itr = cameras.begin(); itr != cameras.end(); itr++)
		{
			if (*itr == camera)
			{
				camera->mainCamera = true;
				mainCamera = (Camera2DEntity*)*itr;
				return;
			}
		}
	}

}