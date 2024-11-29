#pragma once
#include "ve_defines.h"
#include "components/ve_components.h"
namespace VE 
{
	namespace Systems
	{
		enum class SystemPhase
		{
			StartPhase,
			PreUpdatePhase,
			UpdatePhase,
			PosUpdatePhase,
			RenderPhase
		};


		void TransformSystem(flecs::entity e, Components::TransformComponent& tc);
		void Camera2DSystem(flecs::entity e, Components::TransformComponent& transform, Components::Camera2DComponent& c2dc);
		void Sprite2DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::SpriteComponent& sc);

	}
}