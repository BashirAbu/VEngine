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
			glm::vec3 worldScale = { 1.0f,1.0f,1.0f };

			glm::vec3 localPosition = {};
			glm::vec3 localRotation = {};
			glm::vec3 localScale = {1.0f,1.0f,1.0f};
		};

		struct SpriteComponent
		{
			std::filesystem::path texturePath = "";
			glm::vec2 origin = {};
			glm::vec4 tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
			Texture* texture = nullptr;
		};

		struct Camera2DComponent
		{
			glm::vec2 renderTargetSize = {(float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth, (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight};
			glm::vec4 backgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
			float zoom = 1.0f;
			bool isMain = false;
			Camera2D camera = {};
			RenderTexture renderTarget = {};
		};
	}
}