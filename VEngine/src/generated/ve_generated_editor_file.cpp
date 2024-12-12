#ifdef VE_EDITOR
#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "editor/ve_editor.h"
#include <imgui.h>
#include "editor/ve_editor_elements.h"

#include "components/ve_components.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;
using namespace VE::Components::UI;


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
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::SpriteComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			VE::Components::SpriteComponent* SpriteComponent_ = entity.get_mut<VE::Components::SpriteComponent>();
			EditorElement::FileSystem(SpriteComponent_->texturePath, "Texture Path");

			EditorElement::Vec2(SpriteComponent_->origin, "Origin");

			EditorElement::Color(SpriteComponent_->tintColor, "Tint Color");

			EditorElement::Int(SpriteComponent_->renderOrder, "Render Order");

			ImGui::TreePop();
		}
	}
	if (name == "Camera2DComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::Camera2DComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			VE::Components::Camera2DComponent* Camera2DComponent_ = entity.get_mut<VE::Components::Camera2DComponent>();
			EditorElement::Vec2(Camera2DComponent_->renderTargetSize, "Render Target Size");

			EditorElement::Color(Camera2DComponent_->backgroundColor, "Background Color");

			EditorElement::Float(Camera2DComponent_->zoom, "Zoom");

			EditorElement::Checkbox(Camera2DComponent_->isMain, "Is Main");

			ImGui::TreePop();
		}
	}
	if (name == "UICanvasComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UICanvasComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			VE::Components::UI::UICanvasComponent* UICanvasComponent_ = entity.get_mut<VE::Components::UI::UICanvasComponent>();
			EditorElement::Vec2(UICanvasComponent_->canvasSize, "Canvas Size");

			EditorElement::Checkbox(UICanvasComponent_->isMain, "Is Main");

			ImGui::TreePop();
		}
	}
	if (name == "UILabelComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UILabelComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			VE::Components::UI::UILabelComponent* UILabelComponent_ = entity.get_mut<VE::Components::UI::UILabelComponent>();
			EditorElement::FileSystem(UILabelComponent_->fontFilepath, "Font Filepath");

			EditorElement::String(UILabelComponent_->text, "Text");

			EditorElement::Vec2(UILabelComponent_->origin, "Origin");

			EditorElement::Int(UILabelComponent_->renderOrder, "Render Order");

			EditorElement::Color(UILabelComponent_->color, "Color");

			EditorElement::Float(UILabelComponent_->size, "Size");

			ImGui::TreePop();
		}
	}
	if (name == "UIImageComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UIImageComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			VE::Components::UI::UIImageComponent* UIImageComponent_ = entity.get_mut<VE::Components::UI::UIImageComponent>();
			EditorElement::FileSystem(UIImageComponent_->imageFilepath, "Image Filepath");

			EditorElement::Vec2(UIImageComponent_->origin, "Origin");

			EditorElement::Color(UIImageComponent_->tintColor, "Tint Color");

			EditorElement::Int(UIImageComponent_->renderOrder, "Render Order");

			ImGui::TreePop();
		}
	}
	if (name == "UIButtonComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UIButtonComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			VE::Components::UI::UIButtonComponent* UIButtonComponent_ = entity.get_mut<VE::Components::UI::UIButtonComponent>();
			EditorElement::FileSystem(UIButtonComponent_->imageFilepath, "Image Filepath");

			EditorElement::Vec2(UIButtonComponent_->imageOrigin, "Image Origin");

			EditorElement::Color(UIButtonComponent_->tintColor, "Tint Color");

			EditorElement::Color(UIButtonComponent_->pressTintColor, "Press Tint Color");

			EditorElement::Int(UIButtonComponent_->imageRenderOrder, "Image Render Order");

			ImGui::TreePop();
		}
	}

	ImGui::PopID();
}
#endif
