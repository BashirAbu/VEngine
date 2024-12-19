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
		void Camera3DSystem(flecs::entity e, Components::TransformComponent& transform, Components::Camera3DComponent& c3dc);
		void Sprite2DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::SpriteComponent& sc);
		void LightRenderSystem(flecs::iter& it);
		void Mesh3DRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::Model3DComponent& model);



		void UICanvasSystem(flecs::entity e, Components::UI::UICanvasComponent& canvas);
		void UILabelRenderSystem(flecs::entity e, Components::TransformComponent& transform, Components::UI::UILabelComponent& label);
		void UIImageRenderSystem(flecs::entity e, Components::TransformComponent& tc, Components::UI::UIImageComponent& img);
		void UIButtonSystem(flecs::entity e, Components::TransformComponent& tc, Components::UI::UIButtonComponent& button, Components::UI::UIImageComponent& img);
	}
}