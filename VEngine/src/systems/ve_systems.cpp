#include "utils/ve_utils.h"
#include "ve_systems.h"
#include <raylib.h>
#include <raymath.h>
#include <thread>
#include "ve_input.h"
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
			tc.__worldScale= tc.localScale;
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

		if (c2dc.isMain)
		{
			VE::Scene::GetSingleton()->SetMainCamera(e);
		}

		if (glm::vec2(c2dc.renderTarget.texture.width, c2dc.renderTarget.texture.height) != c2dc.renderTargetSize)
		{
			UnloadRenderTexture(c2dc.renderTarget);
			c2dc.renderTarget = LoadRenderTexture((int)c2dc.renderTargetSize.x, (int)c2dc.renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			SetTextureFilter(c2dc.renderTarget.texture, TEXTURE_FILTER_BILINEAR);
		}
	}
	void Camera3DSystem(flecs::entity e, Components::TransformComponent& transform, Components::Camera3DComponent& c3dc)
	{
		c3dc.camera.position = {transform.GetWorldPosition().x, transform.GetWorldPosition().y, transform.GetWorldPosition().z};

		if (c3dc.isMain)
		{
			VE::Scene::GetSingleton()->SetMainCamera(e);
		}

		if (glm::vec2(c3dc.renderTarget.texture.width, c3dc.renderTarget.texture.height) != c3dc.renderTargetSize)
		{
			UnloadRenderTexture(c3dc.renderTarget);
			c3dc.renderTarget = LoadRenderTexture((int)c3dc.renderTargetSize.x, (int)c3dc.renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			SetTextureFilter(c3dc.renderTarget.texture, TEXTURE_FILTER_BILINEAR);
		}
	}
	void Sprite2DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::SpriteComponent& sc)
	{
		if (sc.texturePath != sc.oldTexturePath)
		{
			sc.texture = AssetsManager::GetSingleton()->LoadTexture(sc.texturePath);
		}
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
			

			VE::Renderer::Tex2D tex = { *sc.texture, src, dest, org, tc.GetWorldRotation().z, GLMVec4ToRayColor(sc.tintColor) };
			VE::Scene::GetSingleton()->renderer.Submit(tex, sc.renderOrder, e);
			sc.oldTexturePath = sc.texturePath;
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
		if (img.imageFilepath != img.oldImageFilepath)
		{
			img.texture = AssetsManager::GetSingleton()->LoadTexture(img.imageFilepath);
		}
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


			VE::Renderer::Tex2D tex = { *img.texture, src, dest, org, tc.GetWorldRotation().z, GLMVec4ToRayColor(img.tintColor) };
			VE::Scene::GetSingleton()->renderer.SubmitUI(tex, img.renderOrder, e);
			img.oldImageFilepath = img.imageFilepath;
		}
	}

	void UILabelRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::UI::UILabelComponent& label)
	{
		if (label.fontFilepath != label.oldFontFilepath)
		{
			if (label.font)
			{
				delete label.font;
				label.font = nullptr;
			}
			label.font = AssetsManager::GetSingleton()->LoadFont(label.fontFilepath, (int32_t)label.size);
			//Render
			UnloadTexture(label.texture);

			if (label.font->GetFontSize() != label.size)
				label.font->SetFontSize((int32_t)label.size);
			label.texture = RaylibGetTextureFromText_UTF8(label.font, label.text);
		}
		if (label.font)
		{
			if (label.oldText != label.text || (label.oldTextSize != label.size && label.size > 0))
			{
				UnloadTexture(label.texture);

				if (label.font->GetFontSize() != label.size)
					label.font->SetFontSize((int32_t)label.size);

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
			l2d.texture = label.texture;
			l2d.rotation = tc.GetWorldRotation().z;
			l2d.tint = GLMVec4ToRayColor(label.color);
			label.oldText = label.text;
			label.oldFontFilepath = label.fontFilepath;
			label.oldTextSize = label.size;
			VE::Scene::GetSingleton()->renderer.SubmitUI(l2d, label.renderOrder, e);
		}
	}


	void UIButtonSystem(flecs::entity e, Components::TransformComponent& tc, Components::UI::UIButtonComponent& button)
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
			flecs::entity cameraEntity =  VE::Scene::GetSingleton()->GetMainCamera();

			if (canvas)
			{
				glm::vec2 mousePos = VE::Input::GetMousePosistion();
				glm::vec2 vMousePos = glm::vec2(mousePos.x / VE::Scene::GetSingleton()->renderer.GetMainRenderTarget().texture.width,
					mousePos.y / VE::Scene::GetSingleton()->renderer.GetMainRenderTarget().texture.height);
				vMousePos = glm::vec2(vMousePos.x * canvas->canvasSize.x, vMousePos.y * canvas->canvasSize.y);

				if (button.imgTexture) 
				{
					Rectangle rect = {};
					rect.x = tc.GetWorldPosition().x - button.imageOrigin.x * button.imgTexture->width;
					rect.width = button.imgTexture->width * tc.GetWorldScale().x;
					rect.y = tc.GetWorldPosition().y - button.imageOrigin.y * button.imgTexture->height;
					rect.height = button.imgTexture->height * tc.GetWorldScale().x;

					Vector2 point = {vMousePos.x, vMousePos.y};
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

		Components::UI::UIImageComponent img;

		img.imageFilepath = button.imageFilepath;
		img.oldImageFilepath = button.oldImageFilepath;
		img.origin = button.imageOrigin;
		img.renderOrder = button.imageRenderOrder;
		img.tintColor = button.__down ? button.pressTintColor : button.tintColor;


		if (button.imageFilepath != button.oldImageFilepath)
		{
			button.imgTexture = AssetsManager::GetSingleton()->LoadTexture(button.imageFilepath);
		}
		img.texture = button.imgTexture;

		UIImageRenderSystem(e, tc, img);
		button.oldImageFilepath = button.imageFilepath;
	}
	

}