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

		void TransformSystem(flecs::entity e, Components::TransformComponent& transform);
		void Sprite2DRenderSystem(flecs::iter& it);
	}
}