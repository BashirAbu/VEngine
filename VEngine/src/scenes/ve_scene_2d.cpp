#include "ve_scene_2d.h"
#include "editor/ve_editor.h"
#include "entities/ve_entity.h"
#include "entities/ve_camera_2d_entity.h"
#include "raymath.h"

namespace VE 
{
	Scene2D::Scene2D()
	{
		
		sceneType = SceneType::Scene2D;
	}
	Scene2D::~Scene2D()
	{
		mainCamera = nullptr;
	}
	void Scene2D::Render()
	{
		//for each camera, render the scene once.
		for (Entity* camera : cameras)
		{
			Camera2DEntity* c2d = (Camera2DEntity*)camera;
			BeginTextureMode(*c2d->GetRenderTarget());
			BeginMode2D(c2d->camera2D);

			Color cc = { (uint8_t)(clearColor.r * 255), (uint8_t)(clearColor.g * 255) , (uint8_t)(clearColor.b * 255) , (uint8_t)(clearColor.a * 255) };
			ClearBackground(cc);
			for (Entity* entity : entities)
			{
				entity->Render();
				entity->ComponentsRender();
			}

			EndMode2D();
			EndTextureMode();
		}
	}

	void Scene2D::SetMainCamera(Entity* camera)
	{
		Camera2DEntity* c2de = (Camera2DEntity*)camera;
		for (auto itr = cameras.begin(); itr != cameras.end(); itr++)
		{
			
			((Camera2DEntity*)*itr)->mainCamera = false;
		}
		for (auto itr = cameras.begin(); itr != cameras.end(); itr++)
		{
			if (*itr == c2de)
			{
				c2de->mainCamera = true;
				mainCamera = (Camera2DEntity*)*itr;
			}
		}
	}

	const Entity* Scene2D::GetMainCamera() const
	{
		return mainCamera;
	}

}
