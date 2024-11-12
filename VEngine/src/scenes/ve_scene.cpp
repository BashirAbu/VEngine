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

	Scene::~Scene()
	{
		UnloadRenderTexture(editorCameraRenderTarget);
		for (Entity* entity : entities) 
		{
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
		}
	}
	void Scene::Update(float deltaTime)
	{
		for (Entity* entity : entities)
		{
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