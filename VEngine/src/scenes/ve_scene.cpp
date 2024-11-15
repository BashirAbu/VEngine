#include "ve_scene.h"
#include "entities/ve_entity.h"
#include "entities/ve_camera_2d_entity.h"
#include "ve_engine.h"
#include "editor/ve_editor_input.h"
#include <imgui.h>


namespace VE 
{
	Scene::Scene() 
	{

	}
	void Scene::DeleteEntityChildren(Entity* entity)
	{
		for (auto itr = entity->children.begin(); itr != entity->children.end();)
		{
			if ((*itr)->children.size() > 0)
			{
				DeleteEntityChildren(*itr);
				
				entities.remove(*itr);
				delete* itr;
				itr = entity->children.erase(itr);
				
			}
			else 
			{
				entities.remove(*itr);
				delete* itr;
				itr = entity->children.erase(itr);
			}
		}
	}
	Scene::~Scene()
	{
		UnloadRenderTexture(editorCameraRenderTarget);
		for (Entity* entity : entities) 
		{
			if (entity->children.size() > 0)
			{
				DeleteEntityChildren(entity);
			}
			delete entity;
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
			entity->Update(deltaTime);
			entity->ComponentsUpdate(deltaTime);
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

}