#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "editor/ve_editor.h"
#include <imgui.h>
#include "editor/ve_editor_elements.h"

#include "../../../VEProjects/testPrj/src/components.h"

#include "../../../VEProjects/testPrj/src/systems.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;


extern "C" VE_PROJECT_API void ProjectDrawComponentElements(std::string name, flecs::entity entity)
{
	ImGui::PushID(name.c_str());
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	if (name == "PlayerComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<PlayerComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			PlayerComponent* PlayerComponent_ = entity.get_mut<PlayerComponent>();
			EditorElement::Float(PlayerComponent_->speed, "Speed");

			EditorElement::Vec3(PlayerComponent_->direction, "Direction");

			ImGui::TreePop();
		}
	}
	if (name == "EnemyComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<EnemyComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EnemyComponent* EnemyComponent_ = entity.get_mut<EnemyComponent>();
			EditorElement::Float(EnemyComponent_->speed, "Speed");

			EditorElement::Float(EnemyComponent_->rotationSpeed, "Rotation Speed");

			ImGui::TreePop();
		}
	}
	if (name == "BulletComponent")
	{
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<BulletComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			BulletComponent* BulletComponent_ = entity.get_mut<BulletComponent>();
			EditorElement::Float(BulletComponent_->speed, "Speed");

			ImGui::TreePop();
		}
	}

	ImGui::PopID();
}
