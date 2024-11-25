#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include "ve_engine.h"
#include "ve_assets_manager.h"
#include "utils/ve_utils.h"
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
		struct SceneEntityTag {};
	}

	namespace Components
	{
		struct TransformComponent
		{
			void SetWorldPosition(flecs::entity e, glm::vec3 newPosition) 
			{
				if (!e.parent())
				{
					localPosition = newPosition;
				}
				else
				{
					glm::mat4 parentWorldTransform = VE::GetWorldTransformMatrix(e.parent());
					glm::vec4 worldPos = glm::vec4(newPosition, 1.0f);

					glm::vec4 localPos = glm::inverse(parentWorldTransform) * worldPos;
					localPosition = localPos;
				}
			}
			void SetWorldRotation(flecs::entity e, glm::vec3 newRotation) 
			{
				if (!e.parent())
				{
					localRotation = newRotation;
				}
				else
				{
					glm::mat4 invParentScaleMatrix = glm::inverse(glm::scale(glm::mat4(1.0f), e.parent().get_mut<TransformComponent>()->localScale));

					glm::quat worldRotQuat = glm::quat(glm::vec3(glm::radians(newRotation)));

					glm::quat parentWorldRotQuat = glm::quat_cast(glm::mat3(invParentScaleMatrix * VE::GetWorldTransformMatrix(e.parent())));

					glm::quat localRotQuat = glm::inverse(parentWorldRotQuat) * worldRotQuat;

					glm::vec3 localRotEulerAngles = glm::eulerAngles(localRotQuat);

					localRotation = glm::degrees(localRotEulerAngles);
				}
			}
			void SetWorldScale(flecs::entity e, glm::vec3 newScale) 
			{
				/*if (!e.parent())
				{
					localPosition = newScale;
				}
				else
				{
					glm::mat4 parentWorldTransform = GetWorldTransformMatrix(e.parent());
					glm::vec4 worldScl = glm::vec4(newScale, 1.0f);

					glm::vec4 localScl = glm::inverse(parentWorldTransform) * worldScl;
					localScl = localScl;
				}*/
			}

			glm::vec3 GetWorldPosition(flecs::entity e) 
			{
				if (!e.parent())
				{
					return localPosition;
				}
				else 
				{
					glm::mat4 parentWorldTransform = GetWorldTransformMatrix(e.parent());
					glm::vec4 localPos = glm::vec4(localPosition, 1.0f);

					glm::vec4 worldPos = parentWorldTransform * localPos;
					return worldPos;
				}
			}

			glm::vec3 GetWorldRotation(flecs::entity e) 
			{
				if (!e.parent())
				{
					return localRotation;
				}
				else
				{
					glm::quat localRotQuat = glm::quat(glm::vec3(glm::radians(localRotation)));

					glm::quat parentWorldRotQuat = glm::quat_cast(GetWorldTransformMatrix(e.parent()));

					glm::quat worldRotQuat = parentWorldRotQuat * localRotQuat;

					glm::vec3 worldRotEulerAngles = glm::eulerAngles(worldRotQuat);

					worldRotEulerAngles = glm::degrees(worldRotEulerAngles);
					return worldRotEulerAngles;
				}
			}
			glm::vec3 GetWorldScale(flecs::entity e) 
			{
				if (!e.parent())
				{
					return localScale;
				}
				else
				{
					glm::mat4 parentWorldTransform = GetWorldTransformMatrix(e.parent());
					
					glm::vec3 parentWorldScale;
					parentWorldScale.x = glm::length(glm::vec3(parentWorldTransform[0]));
					parentWorldScale.y = glm::length(glm::vec3(parentWorldTransform[1]));
					parentWorldScale.z = glm::length(glm::vec3(parentWorldTransform[2]));

					glm::mat4 parentWorldScaleMatrix = glm::scale(glm::mat4(1.0f), parentWorldScale);

					glm::vec4 localScl = glm::vec4(localScale, 1.0f);

					glm::vec4 worldScl = parentWorldScaleMatrix * localScl;
					return worldScl;
				}
			}

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