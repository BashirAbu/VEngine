#include "ve_scene_2d.h"
#include "editor/ve_editor.h"
#include "entities/ve_entity.h"
#include "entities/ve_camera_2d_entity.h"
#include "raymath.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
namespace VE 
{
	Scene2D::Scene2D() : clearColor(1.0f, 0.0f, 1.0f, 1.0f)
	{
		editorCamera = {};
		editorCamera.zoom = 1.0f;
		sceneType = SceneType::Scene2D;
	}
	Scene2D::~Scene2D()
	{
		mainCamera = nullptr;
	}
	void Scene2D::Render()
	{
		//EditorCamera
		if (oldRenderTargetSize != Editor::GetSingleton()->GetSceneViewportSize())
		{
			oldRenderTargetSize = Editor::GetSingleton()->GetSceneViewportSize();
			UnloadRenderTexture(editorCameraRenderTarget);
			editorCameraRenderTarget = LoadRenderTexture((int)oldRenderTargetSize.x, (int)oldRenderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			SetTextureFilter(editorCameraRenderTarget.texture, TEXTURE_FILTER_BILINEAR);
		}
		BeginTextureMode(editorCameraRenderTarget);
		BeginMode2D(editorCamera);
		Color cc = { (uint8_t)(clearColor.r * 255), (uint8_t)(clearColor.g * 255) , (uint8_t)(clearColor.b * 255) , (uint8_t)(clearColor.a * 255) };
		ClearBackground(cc);

		for (Entity* entity : entities)
		{
			entity->Render();
			entity->ComponentsRender();
		}

		if (mainCamera)
		{

			std::vector<glm::vec2> vertices = {
												{0.0f                                        , mainCamera->GetRenderTarget()->texture.height}, // Bottom-left
												{mainCamera->GetRenderTarget()->texture.width, mainCamera->GetRenderTarget()->texture.height}, // Bottom-right
												{mainCamera->GetRenderTarget()->texture.width, 0.0f}, // Top-right
												{0.0f,                                         0.0f}  // Top-left
			};

			glm::mat4 transform =
				glm::translate(glm::mat4(1.0f), glm::vec3(mainCamera->camera2D.target.x + mainCamera->camera2D.offset.x, mainCamera->camera2D.target.y + +mainCamera->camera2D.offset.y, 0.0f))
				* glm::rotate(glm::mat4(1.0f), -glm::radians(mainCamera->camera2D.rotation), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::mat4(1.0f), glm::vec3((1.0f / mainCamera->camera2D.zoom), (1.0f / mainCamera->camera2D.zoom), 1.0f))
				* glm::translate(glm::mat4(1.0f), -glm::vec3(mainCamera->camera2D.target.x + mainCamera->camera2D.offset.x, mainCamera->camera2D.target.y + mainCamera->camera2D.offset.y, 0.0f))
				* glm::translate(glm::mat4(1.0f), glm::vec3(mainCamera->camera2D.target.x + mainCamera->camera2D.offset.x, mainCamera->camera2D.target.y + mainCamera->camera2D.offset.y, 0.0f));

			std::vector<glm::vec2> transformedVertices;
			for (const auto& vertex : vertices) {
				glm::vec4 transformedVertex = transform * glm::vec4(vertex, 0.0f, 1.0f);
				transformedVertices.push_back(glm::vec2(transformedVertex));
			}


			DrawLine((int)transformedVertices[0].x, (int)transformedVertices[0].y, (int)transformedVertices[1].x, (int)transformedVertices[1].y, GRAY);
			DrawLine((int)transformedVertices[1].x, (int)transformedVertices[1].y, (int)transformedVertices[2].x, (int)transformedVertices[2].y, GRAY);
			DrawLine((int)transformedVertices[2].x, (int)transformedVertices[2].y, (int)transformedVertices[3].x, (int)transformedVertices[3].y, GRAY);
			DrawLine((int)transformedVertices[3].x, (int)transformedVertices[3].y, (int)transformedVertices[0].x, (int)transformedVertices[0].y, GRAY);
		}
		EndMode2D();
		EndTextureMode();
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
