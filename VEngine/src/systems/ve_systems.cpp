#include "utils/ve_utils.h"
#include "ve_systems.h"
#include <raylib.h>
#include <raymath.h>
#include <thread>
#include "ve_input.h"
#include <rlgl.h>
namespace VE::Systems
{
	void ApplyParentTransform(flecs::entity parent, Components::TransformComponent& parentTC)
	{
		parent.children([&](flecs::entity child)
			{
				glm::mat4 parentWorldTransformMatrix = parentTC.__worldMatrix;
				Components::TransformComponent& childTC = *child.get_mut<Components::TransformComponent>();

				childTC.__localMatrix = glm::translate(glm::mat4(1.0f), childTC.localPosition)
					* glm::rotate(glm::mat4(1.0f), glm::radians(childTC.localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::rotate(glm::mat4(1.0f), glm::radians(childTC.localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
					* glm::rotate(glm::mat4(1.0f), glm::radians(childTC.localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
					* glm::scale(glm::mat4(1.0f), childTC.localScale);

				childTC.__worldMatrix = parentWorldTransformMatrix * childTC.__localMatrix;
				glm::quat rot;
				glm::vec3 skew;
				glm::vec4 proj;
				glm::decompose(childTC.__worldMatrix, childTC.__worldScale, rot, childTC.__worldPosition, skew, proj);
				childTC.__worldRotation = glm::degrees(glm::eulerAngles(rot));
				ApplyParentTransform(child, childTC);
			});
	}
	void TransformSystem(flecs::entity e, Components::TransformComponent& tc)
	{
		//parent apply transformation to children;
		bool hasChildren = false;
		e.children([&](flecs::entity e) { hasChildren = true; return; });
		//single & root enitty
		if (!e.parent())
		{
			tc.__localMatrix = glm::translate(glm::mat4(1.0f), tc.localPosition)
				* glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
				* glm::scale(glm::mat4(1.0f), tc.localScale);
			tc.__worldMatrix = tc.__localMatrix;
			tc.__worldPosition = tc.localPosition;
			tc.__worldRotation = tc.localRotation;
			tc.__worldScale = tc.localScale;
		}
		//root entity only
		if (!e.parent() && hasChildren)
		{
			ApplyParentTransform(e, tc);
		}
	}
	void Camera2DSystem(flecs::entity e, Components::TransformComponent& transform, Components::Camera2DComponent& c2dc)
	{
		c2dc.camera.target.x = transform.GetWorldPosition().x;
		c2dc.camera.target.y = transform.GetWorldPosition().y;

		c2dc.camera.rotation = transform.GetWorldRotation().z;
		c2dc.camera.zoom = c2dc.zoom;
	}
	void Camera3DSystem(flecs::entity e, Components::TransformComponent& transform, Components::Camera3DComponent& c3dc)
	{
		Matrix transformMatrix = GlmMat4ToRaylibMatrix(transform.__worldMatrix);
		c3dc.camera.position.x = transformMatrix.m12;
		c3dc.camera.position.y = transformMatrix.m13;
		c3dc.camera.position.z = transformMatrix.m14;

		Vector3 forward =
		{
			-transformMatrix.m8,
			-transformMatrix.m9,
			-transformMatrix.m10
		};

		c3dc.camera.target = Vector3Add(c3dc.camera.position, forward);

		c3dc.camera.up.x = transformMatrix.m4;
		c3dc.camera.up.y = transformMatrix.m5;
		c3dc.camera.up.z = transformMatrix.m6;

		if (c3dc.isMain)
		{
			Shader* pbrShader = VE::AssetsManager::GetSingleton()->LoadShader("shaders/pbr.glsl");
			int location = GetShaderLocation(*pbrShader, "viewPosition");
			glm::vec3 position = transform.GetWorldPosition();
			SetShaderValue(*pbrShader, location, glm::value_ptr(position), SHADER_UNIFORM_VEC3);
		}
	}
	void Sprite2DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::SpriteComponent& sc)
	{
		if (sc.texture)
		{
			Rectangle src, dest;
			src.x = 0.0f;
			src.width = (float)sc.texture->width;
			src.y = 0.0f;
			src.height = (float)sc.texture->height;
			if (tc.GetWorldScale().x < 0.0f)
			{
				src.width *= -1.0f;
			}
			if (tc.GetWorldScale().y < 0.0f)
			{
				src.height *= -1.0f;
			}

			dest.x = tc.GetWorldPosition().x;
			dest.y = tc.GetWorldPosition().y;
			dest.width = glm::abs(sc.texture->width * tc.GetWorldScale().x);
			dest.height = glm::abs(sc.texture->height * tc.GetWorldScale().y);

			Vector2 org = { dest.width * sc.origin.x, dest.height * sc.origin.y };


			VE::Renderer::Tex2D tex = {};
			tex.texture = &sc.texture;
			tex.source = src;
			tex.dest = dest;
			tex.origin = org;
			tex.rotation = tc.GetWorldRotation().z;
			tex.tint = GLMVec4ToRayColor(sc.tintColor);
			tex.renderOrder = sc.renderOrder;
			tex.entity = e;
			tex.shader = sc.shader ? &sc.shader : nullptr;
			VE::Scene::GetSingleton()->renderer.Submit(tex);
		}
	}

	void LightRenderSystem(flecs::iter& it)
	{
		int lightCount = it.world().query<_Components::LightTag>().count();

		if (lightCount > MAX_LIGHTS)
		{
			return;
		}

		Shader* pbrShader = VE::AssetsManager::GetSingleton()->LoadShader("shaders/pbr.glsl");

		int loc = GetShaderLocation(*pbrShader, "lightCount");
		SetShaderValue(*pbrShader, loc, &lightCount, SHADER_UNIFORM_INT);

		int index = 0;
		while (it.next()) {
			for (auto i : it)
			{
				flecs::entity e = it.entity(i);
				Components::TransformComponent& tc = *e.get_mut<Components::TransformComponent>();
				std::string lightIndexed = "light[" + std::to_string(index) + "]";
				if (e.has<Components::DirectionalLightComponent>())
				{
					Components::DirectionalLightComponent& light = *e.get_mut<Components::DirectionalLightComponent>();
					int location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".type").c_str());
					int type = (int)Components::LightType::Directional;
					SetShaderValue(*light.pbrShader, location, &type, SHADER_UNIFORM_INT);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".color").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.color), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".specular").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.specular), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".ambient").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.ambient), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".position").c_str());
					glm::vec3 position = tc.GetWorldPosition();
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(position), SHADER_UNIFORM_VEC3);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".direction").c_str());
					glm::vec3 forward = glm::normalize(glm::vec3(tc.__worldMatrix[2]));
					forward = -forward;
					glm::vec3 direction = forward;
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(direction), SHADER_UNIFORM_VEC3);
				}
				else if (e.has<Components::PointLightComponent>())
				{
					Components::PointLightComponent& light = *e.get_mut<Components::PointLightComponent>();

					int location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".type").c_str());
					int type = (int)Components::LightType::Point;
					SetShaderValue(*light.pbrShader, location, &type, SHADER_UNIFORM_INT);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".constant").c_str());
					SetShaderValue(*light.pbrShader, location, &light.constant, SHADER_UNIFORM_FLOAT);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".linear").c_str());
					SetShaderValue(*light.pbrShader, location, &light.linear, SHADER_UNIFORM_FLOAT);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".quadratic").c_str());
					SetShaderValue(*light.pbrShader, location, &light.quadratic, SHADER_UNIFORM_FLOAT);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".color").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.color), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".specular").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.specular), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".ambient").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.ambient), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".position").c_str());
					glm::vec3 position = tc.GetWorldPosition();
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(position), SHADER_UNIFORM_VEC3);
				}
				else if (e.has<Components::SpotLightComponent>())
				{
					Components::SpotLightComponent& light = *e.get_mut<Components::SpotLightComponent>();

					int location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".type").c_str());
					int type = (int)Components::LightType::Spot;
					SetShaderValue(*light.pbrShader, location, &type, SHADER_UNIFORM_INT);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".color").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.color), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".specular").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.specular), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".ambient").c_str());
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(light.ambient), SHADER_UNIFORM_VEC4);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".position").c_str());
					glm::vec3 position = tc.GetWorldPosition();
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(position), SHADER_UNIFORM_VEC3);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".direction").c_str());
					glm::vec3 forward = glm::normalize(glm::vec3(tc.__worldMatrix[2]));
					forward = -forward;
					glm::vec3 direction = forward;
					SetShaderValue(*light.pbrShader, location, glm::value_ptr(direction), SHADER_UNIFORM_VEC3);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".cutOff").c_str());
					SetShaderValue(*light.pbrShader, location, &light.cutOff, SHADER_UNIFORM_FLOAT);

					location = GetShaderLocation(*light.pbrShader, (lightIndexed + ".outerCutOff").c_str());
					SetShaderValue(*light.pbrShader, location, &light.outerCutOff, SHADER_UNIFORM_FLOAT);
				}

				index++;
			}
		}
	}


	void SetShaderMaterialValue(_Components::TextureMap& textureMap, MaterialMap& rayMaterialMap)
	{
		rayMaterialMap.color = GLMVec4ToRayColor(textureMap.color);
		rayMaterialMap.value = textureMap.value;
		rayMaterialMap.texture = textureMap.texture;
	}


	void Mesh3DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::Model3DComponent& model)
	{
		if (model.model)
		{
			//TraceLog(LOG_DEBUG, "Number of materials: %d, NumberOfMeshCount: %d", model.model->materialCount, model.model->meshCount);
			model.model->transform = GlmMat4ToRaylibMatrix(tc.__worldMatrix);

			VE::Renderer::Model3D m3d = {};
			m3d.model = &model.model;
			m3d.entity = e;

			for (int i = 0; i < model.materials.size(); i++)
			{
				SetShaderMaterialValue(model.materials[i].albedoMap, model.model->materials[i].maps[MATERIAL_MAP_ALBEDO]);

				SetShaderMaterialValue(model.materials[i].specularMap, model.model->materials[i].maps[MATERIAL_MAP_SPECULAR]);

				SetShaderMaterialValue(model.materials[i].ambientOcclusionMap, model.model->materials[i].maps[MATERIAL_MAP_OCCLUSION]);
			}


			VE::Scene::GetSingleton()->renderer.Submit(m3d);
		}
	}
	void UICanvasSystem(flecs::entity e, Components::UI::UICanvasComponent& canvas)
	{
		if (canvas.isMain)
		{
			flecs::query cavases = VE::Scene::GetSingleton()->GetFlecsWorld().query<Components::UI::UICanvasComponent>();

			cavases.each([&](flecs::entity entity, Components::UI::UICanvasComponent& c)
				{
					if (e == entity)
					{
						c.isMain = true;
					}
					else
					{
						c.isMain = false;
					}
				});
		}

		if (glm::vec2(canvas.canvasRenderTarget.texture.width, canvas.canvasRenderTarget.texture.height) != canvas.canvasSize)
		{
			UnloadRenderTexture(canvas.canvasRenderTarget);
			canvas.canvasRenderTarget = LoadRenderTexture((int)canvas.canvasSize.x, (int)canvas.canvasSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			SetTextureFilter(canvas.canvasRenderTarget.texture, TEXTURE_FILTER_BILINEAR);
		}
	}
	void UIImageRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::UI::UIImageComponent& img)
	{

		if (img.texture)
		{
			Rectangle src, dest;
			src.x = 0.0f;
			src.width = (float)img.texture->width;
			src.y = 0.0f;
			src.height = (float)img.texture->height;
			if (tc.GetWorldScale().x < 0.0f)
			{
				src.width *= -1.0f;
			}
			if (tc.GetWorldScale().y < 0.0f)
			{
				src.height *= -1.0f;
			}

			dest.x = tc.GetWorldPosition().x;
			dest.y = tc.GetWorldPosition().y;
			dest.width = glm::abs(img.texture->width * tc.GetWorldScale().x);
			dest.height = glm::abs(img.texture->height * tc.GetWorldScale().y);

			Vector2 org = { dest.width * img.origin.x, dest.height * img.origin.y };


			VE::Renderer::Tex2D tex = {};
			tex.texture = &img.texture;
			tex.source = src;
			tex.dest = dest;
			tex.origin = org;
			tex.rotation = tc.GetWorldRotation().z;
			tex.tint = GLMVec4ToRayColor(img.tintColor);
			tex.renderOrder = img.renderOrder;
			tex.entity = e;

			VE::Scene::GetSingleton()->renderer.SubmitUI(tex);
		}
	}

	void UILabelRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::UI::UILabelComponent& label)
	{
		if (label.font)
		{
			if (label.oldText != label.text)
			{
				label.texture = RaylibGetTextureFromText_UTF8(label.font, label.text);
			}

			Rectangle src, dest;
			src.x = 0.0f;
			src.width = (float)label.texture.width;
			src.y = 0.0f;
			src.height = (float)label.texture.height;
			if (tc.GetWorldScale().x < 0.0f)
			{
				src.width *= -1.0f;
			}
			if (tc.GetWorldScale().y < 0.0f)
			{
				src.height *= -1.0f;
			}

			dest.x = tc.GetWorldPosition().x;
			dest.y = tc.GetWorldPosition().y;
			dest.width = glm::abs(label.texture.width * tc.GetWorldScale().x);
			dest.height = glm::abs(label.texture.height * tc.GetWorldScale().y);

			Renderer::Tex2D l2d = {};
			l2d.source = src;
			l2d.dest = dest;
			Vector2 org = { dest.width * label.origin.x, dest.height * label.origin.y };
			l2d.origin = org;
			l2d.texture = &label.texturePtr;
			l2d.rotation = tc.GetWorldRotation().z;
			l2d.tint = GLMVec4ToRayColor(label.color);
			l2d.entity = e;
			l2d.renderOrder = label.renderOrder;
			VE::Scene::GetSingleton()->renderer.SubmitUI(l2d);
			label.oldText = label.text;
		}
	}


	void UIButtonSystem(flecs::entity e, Components::TransformComponent& tc, Components::UI::UIButtonComponent& button, Components::UI::UIImageComponent& img)
	{
		//get main canvas.
		flecs::query qCanvas = e.world().query<VE::Components::UI::UICanvasComponent>();

		flecs::entity canvasEntity = qCanvas.find([](flecs::entity e, VE::Components::UI::UICanvasComponent& canvas)
			{
				return canvas.isMain;
			});
		if (canvasEntity)
		{
			VE::Components::UI::UICanvasComponent* canvas = canvasEntity.get_mut<VE::Components::UI::UICanvasComponent>();
			flecs::entity cameraEntity = VE::Scene::GetSingleton()->GetMainCamera();

			if (canvas)
			{
				glm::vec2 mousePos = VE::Input::GetMousePosistion();
				glm::vec2 vMousePos = glm::vec2(mousePos.x / VE::Scene::GetSingleton()->renderer.GetMainRenderTarget().texture.width,
					mousePos.y / VE::Scene::GetSingleton()->renderer.GetMainRenderTarget().texture.height);
				vMousePos = glm::vec2(vMousePos.x * canvas->canvasSize.x, vMousePos.y * canvas->canvasSize.y);

				if (img.texture)
				{
					Rectangle rect = {};
					rect.x = tc.GetWorldPosition().x - img.origin.x * img.texture->width;
					rect.width = img.texture->width * tc.GetWorldScale().x;
					rect.y = tc.GetWorldPosition().y - img.origin.y * img.texture->height;
					rect.height = img.texture->height * tc.GetWorldScale().x;

					Vector2 point = { vMousePos.x, vMousePos.y };
					if (CheckCollisionPointRec(point, rect))
					{
						if (VE::Input::IsMouseButtonDown(MOUSE_BUTTON_LEFT))
						{
							button.__down = true;
						}
						else
						{
							button.__down = false;
						}

						if (VE::Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
						{
							button.imgTintColor = img.tintColor;
							img.tintColor = button.pressTintColor;
						}

						if (VE::Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
						{
							if (button.callback)
							{
								button.callback();
							}
						}
					}
					else
					{
						button.__down = false;

					}
				}
			}
		}

		if (!button.__down)
		{
			img.tintColor = button.imgTintColor;
		}
	}
}