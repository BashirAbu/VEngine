#include "ve_systems.h"
#include "utils/ve_utils.h"
#include <raylib.h>
#include <raymath.h>
#include <thread>
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
	void Camera2DTransformSystem(flecs::entity e, Components::TransformComponent& transform, Components::Camera2DComponent& c2dc)
	{
		c2dc.camera.target.x = transform.GetWorldPosition().x;
		c2dc.camera.target.y = transform.GetWorldPosition().y;

		c2dc.camera.rotation = transform.GetWorldRotation().z;
	}
	void Sprite2DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::SpriteComponent& sc)
	{
		if (!sc.texturePath.empty() && !sc.texture)
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
			

			VE::Renderer::Tex2D tex = { sc.texture, src, dest, org, tc.GetWorldRotation().z, GLMVec4ToRayColor(sc.tintColor) };
			VE::Scene::GetSingleton()->renderer.Submit(tex, sc.renderOrder, e);
			

		}
	}

}