#include "ve_systems.h"
#include "utils/ve_utils.h"
#include <raylib.h>
namespace VE::Systems
{

	
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
			
			DrawTexturePro(*sc.texture, src, dest, org, tc.GetWorldRotation().z, GLMVec4ToRayColor(sc.tintColor));
		}
	}

}