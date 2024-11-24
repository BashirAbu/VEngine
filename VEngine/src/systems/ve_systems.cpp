#include "ve_systems.h"
#include "utils/ve_utils.h"
#include <raylib.h>
namespace VE::Systems
{
	void TransformSystem(flecs::entity e, Components::TransformComponent& transform)
	{
	}
	void Sprite2DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::SpriteComponent& sc)
	{
		if (!sc.texturePath.empty())
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