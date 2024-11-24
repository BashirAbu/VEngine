#include "ve_systems.h"
#include "utils/ve_utils.h"
#include <raylib.h>
namespace VE::Systems
{

	void ApplyParentTransform(flecs::entity e, glm::mat4 parentTransformMatrix) 
	{
		e.children([&](flecs::entity child) 
			{
				Components::TransformComponent* childTransform = child.get_mut<Components::TransformComponent>();
				if (childTransform)
				{
					glm::mat4 childTransformMatrix = GetLocalTransformMatrix(*childTransform);
					glm::mat4 worldTransformMatrix = parentTransformMatrix * childTransformMatrix;
					glm::quat rot;
					glm::vec3 skew;
					glm::vec4 pres;
					glm::decompose(worldTransformMatrix, childTransform->worldScale, rot, childTransform->worldPosition, skew, pres);

					childTransform->worldRotation = glm::degrees(glm::eulerAngles(rot));
					bool hasChildren = false;
					child.children([&](flecs::entity e)
						{
							hasChildren = true;
							return;
						});
					if (hasChildren)
					{
						ApplyParentTransform(child, GetWorldTransformMatrix(*childTransform));
					}
				}
				
			});
	}
	void TransformSystem(flecs::entity e, Components::TransformComponent& transform)
	{
		//update parent child stuff
		bool hasChildren = false;
		e.children([&](flecs::entity e)
			{
				hasChildren = true;
				return;
			});
		//Get root entity
		if (!e.parent() && hasChildren)
		{
			transform.worldPosition = transform.localPosition;
			transform.worldRotation = transform.localRotation;
			transform.worldScale = transform.localScale;
			ApplyParentTransform(e, GetLocalTransformMatrix(transform));
		}
		else if(!e.parent() && !hasChildren)
		{
			transform.worldPosition = transform.localPosition;
			transform.worldRotation = transform.localRotation;
			transform.worldScale = transform.localScale;
		}

	}
	void Camera2DTransformSystem(flecs::entity e, Components::TransformComponent& transform, Components::Camera2DComponent& c2dc)
	{
		c2dc.camera.target.x = transform.worldPosition.x;
		c2dc.camera.target.y = transform.worldPosition.y;

		c2dc.camera.rotation = transform.worldRotation.z;
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
			if (tc.worldScale.x < 0.0f)
			{
				src.width *= -1.0f;
			}
			if (tc.worldScale.y < 0.0f)
			{
				src.height *= -1.0f;
			}

			dest.x = tc.worldPosition.x;
			dest.y = tc.worldPosition.y;
			dest.width = glm::abs(sc.texture->width * tc.worldScale.x);
			dest.height = glm::abs(sc.texture->height * tc.worldScale.y);

			Vector2 org = { dest.width * sc.origin.x, dest.height * sc.origin.y };
			
			DrawTexturePro(*sc.texture, src, dest, org, tc.worldRotation.z, GLMVec4ToRayColor(sc.tintColor));
		}
	}

}