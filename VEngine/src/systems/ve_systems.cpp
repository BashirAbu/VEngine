#include "ve_systems.h"
#include <raylib.h>
namespace VE::Systems
{
	void TransformSystem(flecs::entity e, Components::TransformComponent& transform)
	{
	}
	void Sprite2DRenderSystem(flecs::iter& it)
	{
		flecs::query cameras = it.world().query<Components::Camera2DComponent>();

		cameras.each([&](flecs::entity e, Components::Camera2DComponent& camera)
			{
				BeginDrawing();
				BeginTextureMode(camera.renderTarget);

				BeginMode2D(camera.camera);

				while (it.next())
				{
					auto tc = it.field<Components::TransformComponent>(0);
					auto sc = it.field<Components::SpriteComponent>(1);

					if (sc->texture)
					{
						for (size_t i : it)
						{
							Rectangle src, dest;
							src.x = 0.0f;
							src.width = (float)sc[i].texture->width;
							src.y = 0.0f;
							src.height = (float)sc[i].texture->height;
							if (tc[i].worldScale.x < 0.0f)
							{
								src.width *= -1.0f;
							}
							if (tc[i].worldScale.y < 0.0f)
							{
								src.height *= -1.0f;
							}

							dest.x = tc[i].worldPosition.x;
							dest.y = tc[i].worldPosition.y;
							dest.width = glm::abs(sc[i].texture->width * tc[i].worldScale.x);
							dest.height = glm::abs(sc[i].texture->height * tc[i].worldScale.y);

							Vector2 org = { dest.width * sc[i].origin.x, dest.height * sc[i].origin.y };
							Color col = { uint8_t(sc[i].tintColor.r * 255.0f), uint8_t(sc[i].tintColor.g * 255.0f), uint8_t(sc[i].tintColor.b * 255.0f), uint8_t(sc[i].tintColor.a * 255.0f) };
							DrawTexturePro(*sc[i].texture, src, dest, org, tc[i].worldRotation.z, col);
						}
					}


				}

				EndMode2D();

				EndTextureMode();
				EndDrawing();
			});
	}
}