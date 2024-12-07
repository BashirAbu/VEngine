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
		struct OnRender {};
		struct SceneEntityTag {};
		struct SceneEntityUITag {};
		struct ConstructTag {};
	}


	namespace Components
	{
		VE_COMPONENT()
		struct TransformComponent
		{
			TransformComponent() : __localMatrix(1.0f), __worldMatrix(1.0f)
			{}
			void SetWorldPosition(glm::vec3 newPosition) 
			{
				if (!e.parent())
				{
					localPosition = newPosition;
				}
				else
				{
					TransformComponent* parentTC = e.parent().get_mut<TransformComponent>();

					glm::mat4 parentWorldTransform = parentTC->__worldMatrix;

					glm::vec4 worldPos = glm::vec4(newPosition, 1.0f);

					glm::vec4 localPos = glm::inverse(parentWorldTransform) * worldPos;
					localPosition = localPos;
				}
			}
			void SetWorldRotation(glm::vec3 newRotation) 
			{
				if (!e.parent())
				{
					localRotation = newRotation;
				}
				else
				{
					glm::mat4 invParentScaleMatrix = glm::inverse(glm::scale(glm::mat4(1.0f), e.parent().get_mut<TransformComponent>()->localScale));

					glm::quat worldRotQuat = glm::quat(glm::vec3(glm::radians(newRotation)));

					TransformComponent* parentTC = e.parent().get_mut<TransformComponent>();

					glm::mat4 parentWorldTransform = parentTC->__worldMatrix;

					glm::quat parentWorldRotQuat = glm::quat_cast(glm::mat3(invParentScaleMatrix * parentWorldTransform));

					glm::quat localRotQuat = glm::inverse(parentWorldRotQuat) * worldRotQuat;

					glm::vec3 localRotEulerAngles = glm::eulerAngles(localRotQuat);

					localRotation = glm::degrees(localRotEulerAngles);
				}
			}
			void SetWorldScale(glm::vec3 newScale) 
			{
				if (!e.parent())
				{
					localScale = newScale;
				}
				else
				{
					TransformComponent* parentTC = e.parent().get_mut<TransformComponent>();

					glm::mat4 parentWorldTransform = parentTC->__worldMatrix;

					glm::vec3 parentWorldScale;
					parentWorldScale.x = glm::length(glm::vec3(parentWorldTransform[0]));
					parentWorldScale.y = glm::length(glm::vec3(parentWorldTransform[1]));
					parentWorldScale.z = glm::length(glm::vec3(parentWorldTransform[2]));

					glm::mat4 parentWorldScaleMatrix = glm::scale(glm::mat4(1.0f), parentWorldScale);


					glm::vec4 worldScl = glm::vec4(newScale, 1.0f);

					glm::vec4 localScl = glm::inverse(parentWorldScaleMatrix) * worldScl;
					localScl = localScl;
				}
			}

			glm::vec3 GetWorldPosition() 
			{
				if (!e.parent())
				{
					return localPosition;
				}
				else 
				{
					return __worldPosition;
				}
			}

			glm::vec3 GetWorldRotation() 
			{
				if (!e.parent())
				{
					return localRotation;
				}
				else
				{
					return __worldRotation;
				}
			}
			glm::vec3 GetWorldScale() 
			{
				if (!e.parent())
				{
					return localScale;
				}
				else
				{
					
					return __worldScale;
				}
			}
			VE_PROPERTY(Editor)
			glm::vec3 localPosition = {};
			VE_PROPERTY(Editor)
			glm::vec3 localRotation = {};
			VE_PROPERTY(Editor)
			glm::vec3 localScale = {1.0f,1.0f,1.0f};
			flecs::entity e;

			//internal
			//read write
			glm::mat4 __localMatrix;
			//read only
			glm::mat4 __worldMatrix;
			//read only
			glm::vec3 __worldPosition = {};
			//read only
			glm::vec3 __worldRotation = {};
			//read only
			glm::vec3 __worldScale = { 1.0f,1.0f,1.0f };
		};

		VE_COMPONENT()
		struct SpriteComponent
		{
			VE_PROPERTY(Editor)
			std::filesystem::path texturePath = "";
			std::filesystem::path oldTexturePath = "";
			VE_PROPERTY(Editor)
			glm::vec2 origin = {};
			VE_PROPERTY(Editor)
			NormalizedColor tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
			VE_PROPERTY(Editor)
			int32_t renderOrder = 0;
			Texture* texture = nullptr;
		};


		VE_COMPONENT()
		struct Camera2DComponent
		{
			Camera2D camera = {};
			RenderTexture renderTarget = {};
			VE_PROPERTY(Editor)
			glm::vec2 renderTargetSize = {(float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
										  (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight};
			VE_PROPERTY(Editor)
			NormalizedColor backgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
			VE_PROPERTY(Editor)
			float zoom = 1.0f;
			VE_PROPERTY(Editor)
			bool isMain = false;
		};


		namespace UI
		{
			VE_COMPONENT()
			struct UICanvasComponent 
			{
				RenderTexture canvasRenderTarget = {};
				VE_PROPERTY(Editor)
				glm::vec2 canvasSize = { (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
										  (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight };
				VE_PROPERTY(Editor)
				bool isMain = false;

			};

			VE_COMPONENT()
			struct LabelComponent 
			{
				class VE::Font* font = nullptr;
				VE_PROPERTY(Editor)
				std::filesystem::path fontFilepath = "";
				std::filesystem::path oldFontFilepath = "";
				VE_PROPERTY(Editor)
				std::string text = "";
				VE_PROPERTY(Editor);
				glm::vec2 origin = {};
				VE_PROPERTY(Editor)
				int32_t renderOrder = 0;
				VE_PROPERTY(Editor)
				NormalizedColor color = {1.0f, 1.0f, 1.0f, 1.0f};
				VE_PROPERTY(Editor)
				float size = 12;
				VE_PROPERTY(Editor)
				float spacing = 1.0f;
				std::string oldText = "";
				Texture2D texture = {};
			};
		}
	}
}