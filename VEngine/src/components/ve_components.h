#pragma once
#include "ve_defines.h"
#include <raylib.h>
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

		struct Color
		{
			union
			{
				glm::vec4 elements;
				struct
				{
					float r, g, b, a;
				};
			};
		};
		struct SpriteComponent
		{
			Texture* texture = nullptr;
			std::filesystem::path texturePath = "";
			glm::vec2 origin = {};
			Color tintColor = {};
		};

		struct Camera2DComponent
		{
			Camera2D camera;
			RenderTexture renderTarget;
			bool isMain = false;
		};
	}
}