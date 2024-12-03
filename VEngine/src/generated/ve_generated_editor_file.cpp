#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "editor/ve_editor.h"
#include <imgui.h>
#include "editor/ve_editor_elements.h"

#include "components/ve_components.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;


void VE::Editor::DrawComponentElements(std::string name, flecs::entity entity)
{
	ImGui::PushID(name.c_str());
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	if (name == "TransformComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (open)
		{
			
			VE::Components::TransformComponent* TransformComponent_ = entity.get_mut<VE::Components::TransformComponent>();
			EditorElement::Vec3(TransformComponent_->localPosition, "Local Position");

			EditorElement::Vec3(TransformComponent_->localRotation, "Local Rotation");

			EditorElement::Vec3(TransformComponent_->localScale, "Local Scale");

			ImGui::TreePop();
		}
	}
	if (name == "SpriteComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (open)
		{
			
			VE::Components::SpriteComponent* SpriteComponent_ = entity.get_mut<VE::Components::SpriteComponent>();
			EditorElement::FileSystem(SpriteComponent_->texturePath, "Texture Path");

			EditorElement::Vec2(SpriteComponent_->origin, "Origin");

			EditorElement::Color(SpriteComponent_->tintColor, "Tint Color");

			EditorElement::Int(SpriteComponent_->renderOrder, "Render Order");

			ImGui::TreePop();
		}
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::SpriteComponent>(); open = false;
			}
			ImGui::EndPopup();
		}
	}
	if (name == "Camera2DComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (open)
		{
			
			VE::Components::Camera2DComponent* Camera2DComponent_ = entity.get_mut<VE::Components::Camera2DComponent>();
			EditorElement::Vec2(Camera2DComponent_->renderTargetSize, "Render Target Size");

			EditorElement::Color(Camera2DComponent_->backgroundColor, "Background Color");

			EditorElement::Float(Camera2DComponent_->zoom, "Zoom");

			EditorElement::Checkbox(Camera2DComponent_->isMain, "Is Main");

			ImGui::TreePop();
		}
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::Camera2DComponent>(); open = false;
			}
			ImGui::EndPopup();
		}
	}

	ImGui::PopID();
}
