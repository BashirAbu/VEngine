#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include "ve_engine.h"
#include "ve_assets_manager.h"
namespace VE 
{

	namespace _Components 
	{
		//System phases
		struct StartPhase {};
		struct PreUpdatePhase {};
		struct UpdatePhase {};
		struct PostUpdatePhase {};
		struct RenderPhase {};
		struct SceneTag {};
	}

	namespace Components
	{



		struct TransformComponent
		{
			glm::vec3 worldPosition = {};
			glm::vec3 worldRotation = {};
			glm::vec3 worldScale = {};

			glm::vec3 localPosition = {};
			glm::vec3 localRotation = {};
			glm::vec3 localScale = {};
		};

		struct SpriteComponent
		{

			SpriteComponent() 
			{
				if (!texturePath.empty())
				{
					texture = VE::AssetsManager::GetSingleton()->LoadTexture(texturePath);
				}
			}
			std::filesystem::path texturePath = "";
			glm::vec2 origin = {};
			glm::vec4 tintColor = {};
			Texture* texture = nullptr;
		};

		struct Camera2DComponent
		{
			Camera2DComponent() 
			{
				renderTarget = LoadRenderTexture(VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
					VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight,
					PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			}
			~Camera2DComponent() 
			{
				UnloadRenderTexture(renderTarget);
			}
			Camera2D camera = {};
			RenderTexture renderTarget = {};
			bool isMain = false;
		};
	}
}