#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include "ve_engine.h"
#include "ve_assets_manager.h"
#include "utils/ve_utils.h"
#include <rlgl.h>
#define MAX_LIGHTS 10

namespace VE
{

	namespace _Components
	{
		struct OnRender {};
		struct SceneEntityTag {};
		struct SceneEntityUITag {};
		struct ConstructTag {};
		struct Disabled {};

		struct LightTag {};

		VE_ENUM()
			enum class BasicMesh
		{
			None = 0,
			Cube,
			Sphere,
		};

		VE_CLASS(Component)
			struct TextureMap
		{
			VE_PROPERTY(Editor)
				NormalizedColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
			VE_PROPERTY(Editor)
				float value;
			VE_PROPERTY(Editor, OnChange = TextureMapTexturePathOnChange)
				std::filesystem::path texturePath = "";
			VE_PROPERTY(Editor)
				Texture texture = {};
		};

		VE_FUNCTION(Callback)
			inline void TextureMapTexturePathOnChange(void* data)
		{
			TextureMap* tm = (TextureMap*)data;
			if (tm)
			{
				tm->texture = *VE::AssetsManager::GetSingleton()->LoadTexture(tm->texturePath);
			}
		}

		VE_CLASS(Component)
			struct VEMaterial
		{
			VE_PROPERTY(Editor)
				TextureMap albedoMap;
			VE_PROPERTY(Editor)
				TextureMap specularMap;
			VE_PROPERTY(Editor)
				TextureMap ambientOcclusionMap;

			Shader* shader;
		};

	}


	namespace Components
	{
		VE_CLASS(Component)
			struct TransformComponent
		{
			TransformComponent() : __localMatrix(1.0f), __worldMatrix(1.0f)
			{
			}
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
				glm::vec3 localScale = { 1.0f,1.0f,1.0f };
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
				NormalizedColor tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
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
			VE_PROPERTY(Editor, OnChange = Camera2DComponentRenderTargetSizeOnChange)
				glm::vec2 renderTargetSize = { (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
											  (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight };
			VE_PROPERTY(Editor)
				NormalizedColor backgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			VE_PROPERTY(Editor)
				float zoom = 1.0f;
			VE_PROPERTY(Editor, OnChange = Camera2DComponentRenderIsMainOnChange)
				bool isMain = false;
		};

		VE_FUNCTION(Callback)
			inline void Camera2DComponentRenderTargetSizeOnChange(void* data)
		{
			Camera2DComponent* camera = (Camera2DComponent*)data;
			if (camera)
			{
				UnloadRenderTexture(camera->renderTarget);

				camera->renderTarget = LoadRenderTexture((int)camera->renderTargetSize.x, (int)camera->renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
				SetTextureFilter(camera->renderTarget.texture, TEXTURE_FILTER_BILINEAR);
			}
		}

		VE_FUNCTION(Callback)
			inline void Camera2DComponentRenderIsMainOnChange(void* data)
		{
			Camera2DComponent* camera = (Camera2DComponent*)data;
			if (camera)
			{
				if (camera->isMain)
				{
					flecs::entity cameraEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<Camera2DComponent>().find([&](flecs::entity e, Camera2DComponent& c2dc)
						{
							return &c2dc == camera;
						});

					if (cameraEntity)
					{
						VE::Scene::GetSingleton()->SetMainCamera(cameraEntity);
					}
				}
			}
		}
		VE_CLASS(Component)
			struct Camera3DComponent
		{
			Camera camera = {};
			RenderTexture renderTarget = {};
			VE_PROPERTY(Editor, OnChange = Camera3DComponentRenderTargetSizeOnChange)
				glm::vec2 renderTargetSize = { (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
												  (float)VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight };

			VE_PROPERTY(Editor)
				NormalizedColor backgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			VE_PROPERTY(Editor)
				float zoom = 1.0f;
			VE_PROPERTY(Editor, OnChange = Camera3DComponentRenderIsMainOnChange)
				bool isMain = false;
			VE_PROPERTY(Editor, OnChange = Camera3DComponentSkyboxTexturePathOnChange)
				std::filesystem::path skyboxTexturePath = "";
			VE_PROPERTY(Editor)
				Texture skyboxTexture = {};
			Model cubeMapModel = {};
			Shader* skyboxShader = nullptr;
		};

		inline void LoadSkymap(Components::Camera3DComponent* camera)
		{
			if (camera)
			{
				camera->skyboxTexture = *VE::AssetsManager::GetSingleton()->LoadTexture(camera->skyboxTexturePath);
				if (!camera->skyboxTexturePath.empty())
				{
					camera->skyboxTexture = *VE::AssetsManager::GetSingleton()->LoadTexture(camera->skyboxTexturePath);

					camera->skyboxShader = VE::AssetsManager::GetSingleton()->LoadShader("shaders/skybox.glsl");

					int loc = rlGetLocationAttrib(camera->skyboxShader->id, "vertexPosition");
					camera->skyboxShader->locs[RL_SHADER_LOC_VERTEX_POSITION] = loc;

					loc = GetShaderLocation(*camera->skyboxShader, "projectionMatrix");
					camera->skyboxShader->locs[RL_SHADER_LOC_MATRIX_PROJECTION] = loc;

					loc = GetShaderLocation(*camera->skyboxShader, "viewMatrix");
					camera->skyboxShader->locs[RL_SHADER_LOC_MATRIX_VIEW] = loc;


					loc = GetShaderLocation(*camera->skyboxShader, "skyBox");
					camera->skyboxShader->locs[RL_SHADER_LOC_MAP_CUBEMAP] = loc;

					camera->cubeMapModel = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

					camera->cubeMapModel.materials[0].shader = *camera->skyboxShader;

					Image skymapImage = LoadImageFromTexture(camera->skyboxTexture);
					camera->cubeMapModel.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(skymapImage, CUBEMAP_LAYOUT_AUTO_DETECT);;
					UnloadImage(skymapImage);
				}
			}
		}

		VE_FUNCTION(Callback)
			inline void Camera3DComponentSkyboxTexturePathOnChange(void* data)
		{
			Camera3DComponent* camera = (Camera3DComponent*)data;
			LoadSkymap(camera);
		}

		VE_FUNCTION(Callback)
			inline void Camera3DComponentRenderTargetSizeOnChange(void* data)
		{
			Camera3DComponent* camera = (Camera3DComponent*)data;
			if (camera)
			{
				UnloadRenderTexture(camera->renderTarget);

				camera->renderTarget = LoadRenderTexture((int)camera->renderTargetSize.x, (int)camera->renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
				SetTextureFilter(camera->renderTarget.texture, TEXTURE_FILTER_BILINEAR);
			}
		}

		VE_FUNCTION(Callback)
			inline void Camera3DComponentRenderIsMainOnChange(void* data)
		{
			Camera3DComponent* camera = (Camera3DComponent*)data;
			if (camera)
			{
				if (camera->isMain)
				{
					flecs::entity cameraEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<Camera3DComponent>().find([&](flecs::entity e, Camera3DComponent& c3dc)
						{
							return &c3dc == camera;
						});

					if (cameraEntity)
					{
						VE::Scene::GetSingleton()->SetMainCamera(cameraEntity);
					}
				}
			}
		}


		enum class LightType
		{
			Directional = 0,
			Point = 1,
			Spot = 2
		};

		VE_CLASS(Component)
			struct DirectionalLightComponent
		{
			VE_PROPERTY(Editor)
				NormalizedColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
			VE_PROPERTY(Editor)
				NormalizedColor ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
			VE_PROPERTY(Editor)
				NormalizedColor specular = { 1.0f, 1.0f, 1.0f, 1.0f };
			Shader* pbrShader = nullptr;
		};

		VE_CLASS(Component)
			struct PointLightComponent
		{
			VE_PROPERTY(Editor)
				NormalizedColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
			VE_PROPERTY(Editor)
				NormalizedColor ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
			VE_PROPERTY(Editor)
				NormalizedColor specular = { 1.0f, 1.0f, 1.0f, 1.0f };
			VE_PROPERTY(Editor)
				float constant = 1.0f;
			VE_PROPERTY(Editor)
				float linear = 0.09f;
			VE_PROPERTY(Editor)
				float quadratic = 0.032f;
			Shader* pbrShader = nullptr;
		};

		VE_CLASS(Component)
			struct SpotLightComponent
		{
			VE_PROPERTY(Editor)
				NormalizedColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
			VE_PROPERTY(Editor)
				NormalizedColor ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
			VE_PROPERTY(Editor)
				NormalizedColor specular = { 1.0f, 1.0f, 1.0f, 1.0f };
			VE_PROPERTY(Editor)
				float cutOff = glm::cos(glm::radians(12.5f));
			VE_PROPERTY(Editor)
				float outerCutOff = glm::cos(glm::radians(17.5f));
			Shader* pbrShader = nullptr;
		};

		VE_CLASS(Component)
			struct Model3DComponent
		{
			VE_PROPERTY(Editor, OnChange = BasicModelOnChange)
				_Components::BasicMesh basicMesh = _Components::BasicMesh::None;
			Model* model = nullptr;
			VE_PROPERTY(Editor, OnChange = ModelOnChange)
				std::filesystem::path modelFilepath = "";
			VE_PROPERTY(Editor)
				std::vector<_Components::VEMaterial> materials;
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

		inline Model LoadBasicMesh(_Components::BasicMesh basicMesh)
		{
			Model basicModel = {};
			switch (basicMesh)
			{
			case _Components::BasicMesh::Cube:
			{
				basicModel = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
			}break;
			case _Components::BasicMesh::Sphere:
			{
				basicModel = LoadModelFromMesh(GenMeshSphere(1.0f, 32, 32));
			}break;
			}

			return basicModel;
		}

		inline void LoadModelMaterials(Model3DComponent& model, Shader* shader)
		{
			if ((int)model.materials.size() != model.model->materialCount)
			{
				model.materials = std::vector<_Components::VEMaterial>(model.model->materialCount);
			}


			std::function<void(_Components::TextureMap& textureMap, MaterialMap& rayMaterialMap)> loadTextureMaps = [](_Components::TextureMap& textureMap, MaterialMap& rayMaterialMap)
				{

					if (textureMap.texturePath.empty())
					{
						if (rayMaterialMap.texture.id > 0)
						{
							textureMap.texture = rayMaterialMap.texture;
						}
					}
					else
					{
						textureMap.texture = *AssetsManager::GetSingleton()->LoadTexture(textureMap.texturePath);
						rayMaterialMap.texture = textureMap.texture;
					}
				};

			for (size_t i = 0; i < model.model->materialCount; i++)
			{
				model.materials[i].shader = shader;
				model.model->materials[i].shader = *shader;

				loadTextureMaps(model.materials[i].albedoMap, model.model->materials[i].maps[MATERIAL_MAP_ALBEDO]);
				loadTextureMaps(model.materials[i].specularMap, model.model->materials[i].maps[MATERIAL_MAP_SPECULAR]);
				loadTextureMaps(model.materials[i].ambientOcclusionMap, model.model->materials[i].maps[MATERIAL_MAP_OCCLUSION]);
			}
		}

		VE_FUNCTION(Callback)
			inline void BasicModelOnChange(void* data)
		{
			if (!data) return;

			Model3DComponent* comp = (Model3DComponent*)data;

			if (comp->basicMesh != _Components::BasicMesh::None)
			{
				if (comp->model)
				{
					if (IsModelValid(*comp->model))
					{
						UnloadModel(*comp->model);
					}
				}
				else
				{
					comp->model = (Model*)malloc(sizeof(Model));
				}

				memset(comp->model, 0, sizeof(Model));
				Model temp = LoadBasicMesh(comp->basicMesh);
				memcpy(comp->model, &temp, sizeof(Model));
				LoadModelMaterials(*comp, AssetsManager::GetSingleton()->LoadShader("shaders/pbr.glsl"));
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
					NormalizedColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
				VE_PROPERTY(Editor, OnChange = UILabelSizeOnChange)
					float size = 12;
				Texture2D texture = {};
				Texture2D* texturePtr = nullptr;
				std::string oldText = "";
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