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
		struct Disabled {};
	}


	namespace Components
	{
		VE_CLASS(Component)
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
					localScale = localScl;
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

		VE_CLASS(Component)
		struct SpriteComponent
		{
			VE_PROPERTY(Editor, OnChange = SpriteComponentTextureOnChange)
			std::filesystem::path texturePath = "";
			VE_PROPERTY(Editor)
			glm::vec2 origin = {};
			VE_PROPERTY(Editor)
			NormalizedColor tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
			VE_PROPERTY(Editor)
			int32_t renderOrder = 0;
			Texture* texture = nullptr;


			VE_PROPERTY(Editor, OnChange = SpriteComponentShaderOnChange)
			std::filesystem::path shaderPath = "";
			Shader* shader = nullptr;
		};

		VE_FUNCTION(Callback);
		inline void SpriteComponentTextureOnChange(void* data) 
		{
			SpriteComponent* sc = (SpriteComponent*)data;

			if (sc)
			{
				if (!sc->texturePath.empty())
				{
					sc->texture = AssetsManager::GetSingleton()->LoadTexture(sc->texturePath);
				}
			}
		}

		VE_FUNCTION(Callback);
		inline void SpriteComponentShaderOnChange(void* data)
		{
			SpriteComponent* sc = (SpriteComponent*)data;

			if (!sc->shaderPath.empty())
			{
				sc->shader = AssetsManager::GetSingleton()->LoadShader(sc->shaderPath);
			}
		}

		VE_CLASS(Component)
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

		VE_CLASS(Component)
		struct Camera3DComponent
		{
			Camera camera = {};
			RenderTexture renderTarget = {};
			VE_PROPERTY(Editor)
			glm::vec2 renderTargetSize = { (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
											  (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight };
			VE_PROPERTY(Editor)
			std::filesystem::path skyboxTexturePath = "";
			std::filesystem::path oldSkyboxTexturePath = "";
			Texture* skyboxTexture = nullptr;
			Shader* skyboxShader = nullptr;
			Shader* cubemapShader = nullptr;
			Model skyboxModel = {};

			VE_PROPERTY(Editor)
			NormalizedColor backgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			VE_PROPERTY(Editor)
			float zoom = 1.0f;
			VE_PROPERTY(Editor)
			bool isMain = false;
		};

		VE_ENUM()
		enum class BasicMesh
		{
			None = 0,
			Cube,
			Sphere,
		};


		VE_CLASS(Component)
		struct Model3DComponent
		{
			VE_PROPERTY(Editor, OnChange = BasicModelOnChange)
			BasicMesh basicMesh = BasicMesh::None;
			Model basicModel = {};
			Model* model = nullptr;
			VE_PROPERTY(Editor, OnChange = ModelOnChange)
			std::filesystem::path modelFilepath = "";
		};

		VE_FUNCTION(Callback)
		inline void ModelOnChange(void* data)
		{
			if (!data) return;

			Model3DComponent* comp = (Model3DComponent*)data;

			if (!comp->modelFilepath.empty())
			{
				comp->model = AssetsManager::GetSingleton()->LoadModel(comp->modelFilepath);
			}

		}

		inline Model LoadBasicMesh(BasicMesh basicMesh) 
		{
			Model basicModel = {};
			switch (basicMesh)
			{
			case BasicMesh::Cube:
			{
				basicModel = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
			}break;
			case BasicMesh::Sphere:
			{
				basicModel = LoadModelFromMesh(GenMeshSphere(1.0f, 12, 12));
			}break;
			}

			return basicModel;
		}

		VE_FUNCTION(Callback)
		inline void BasicModelOnChange(void* data)
		{
			if (!data) return;

			Model3DComponent* comp = (Model3DComponent*)data;

			if (comp->basicMesh != BasicMesh::None)
			{
				if (IsModelValid(comp->basicModel)) 
				{
					UnloadModel(comp->basicModel);
				}

				comp->basicModel = LoadBasicMesh(comp->basicMesh);
				comp->model = &comp->basicModel;
			}
		}
		

		namespace UI
		{
			VE_CLASS(Component)
			struct UICanvasComponent 
			{
				RenderTexture canvasRenderTarget = {};
				VE_PROPERTY(Editor)
				glm::vec2 canvasSize = { (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
										 (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight };
				VE_PROPERTY(Editor)
				bool isMain = false;

			};

			VE_CLASS(Component)
			struct UILabelComponent 
			{
				class VE::Font* font = nullptr;
				VE_PROPERTY(Editor, OnChange = UILabelFontOnChange)
				std::filesystem::path fontFilepath = "";
				VE_PROPERTY(Editor, OnChange = UILabelTextOnChange)
				std::string text = "";
				VE_PROPERTY(Editor);
				glm::vec2 origin = {};
				VE_PROPERTY(Editor)
				int32_t renderOrder = 0;
				VE_PROPERTY(Editor)
				NormalizedColor color = {1.0f, 1.0f, 1.0f, 1.0f};
				VE_PROPERTY(Editor, OnChange = UILabelSizeOnChange)
				float size = 12;
				Texture2D texture = {};
				Texture2D* texturePtr = nullptr;
			};

			VE_FUNCTION(Callback)
			inline void UILabelFontOnChange(void* data) 
			{
				UILabelComponent* label = (UILabelComponent*)data;

				if (label)
				{
					if (label->font)
					{
						delete label->font;
						label->font = nullptr;
					}

					label->font = AssetsManager::GetSingleton()->LoadFont(label->fontFilepath, (int32_t)label->size);
					if (label->texture.id > 0)
					{
						UnloadTexture(label->texture);
						label->texturePtr = nullptr;
					}

					label->texture = RaylibGetTextureFromText_UTF8(label->font, label->text);
					label->texturePtr = &label->texture;
				}
			}
			VE_FUNCTION(Callback)
			inline void UILabelSizeOnChange(void* data) 
			{
				UILabelComponent* label = (UILabelComponent*)data;

				if (label)
				{
					if (label->font)
					{
						label->font->SetFontSize((int32_t)label->size);

						if (label->texture.id > 0)
						{
							UnloadTexture(label->texture);
							label->texturePtr = nullptr;
						}

						label->texture = RaylibGetTextureFromText_UTF8(label->font, label->text);
						label->texturePtr = &label->texture;
					}
				}
			}

			VE_FUNCTION(Callback)
			inline void UILabelTextOnChange(void* data)
			{
				UILabelComponent* label = (UILabelComponent*)data;
				if (label)
				{
					if (label->font)
					{
						if (label->texture.id > 0)
						{
							UnloadTexture(label->texture);
							label->texturePtr = nullptr;
						}
						label->texture = RaylibGetTextureFromText_UTF8(label->font, label->text);
						label->texturePtr = &label->texture;
					}
				}
			}


			VE_CLASS(Component)
			struct UIImageComponent
			{
				VE_PROPERTY(Editor, OnChange = UIImageComponentTextureOnChange)
				std::filesystem::path imageFilepath = "";
				VE_PROPERTY(Editor)
				glm::vec2 origin = {};
				VE_PROPERTY(Editor)
				NormalizedColor tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				VE_PROPERTY(Editor)
				int32_t renderOrder = 0;
				Texture* texture = nullptr;
			};

			VE_FUNCTION(Callback)
			inline void UIImageComponentTextureOnChange(void* data) 
			{
				UIImageComponent* img = (UIImageComponent*)data;
				if (img)
				{
					img->texture = AssetsManager::GetSingleton()->LoadTexture(img->imageFilepath);
				}
			}

			VE_CLASS(Component)
			struct UIButtonComponent
			{
				VE_PROPERTY(Editor)
				NormalizedColor pressTintColor = { .78f, .78f, .78f, 1.0f };
				NormalizedColor imgTintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				std::function<void()> callback = nullptr;

				bool __down = false;
			};
		}
	}
}