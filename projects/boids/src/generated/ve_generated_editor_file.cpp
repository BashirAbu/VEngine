#ifdef VE_EDITOR
#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "editor/ve_editor.h"
#include <imgui.h>
#include "editor/ve_editor_elements.h"
#include "components/ve_components.h"

#include "components.h"

#include "systems.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;
using namespace VE::Components::UI;


void DrawManagerComponentEditorUI(std::string name, flecs::entity entity, ManagerComponent* ManagerComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(578)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<ManagerComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Float(ManagerComponent_->speed, "Speed");


			EditorElement::Float(ManagerComponent_->radius, "Radius");


			EditorElement::Float(ManagerComponent_->alignment, "Alignment");


			EditorElement::Float(ManagerComponent_->seperation, "Seperation");


			EditorElement::Float(ManagerComponent_->cohesion, "Cohesion");


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawBoidComponentEditorUI(std::string name, flecs::entity entity, BoidComponent* BoidComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(417)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<BoidComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			ImGui::TreePop();
		}
		ImGui::PopID();
}
extern "C" VE_PROJECT_API void ProjectDrawComponentElements(std::string name, flecs::entity entity)
{
	if(name == "ManagerComponent"){
			ManagerComponent* ManagerComponent_ = entity.get_mut<ManagerComponent>();
		DrawManagerComponentEditorUI(name, entity,ManagerComponent_, true);
	}
	if(name == "BoidComponent"){
			BoidComponent* BoidComponent_ = entity.get_mut<BoidComponent>();
		DrawBoidComponentEditorUI(name, entity,BoidComponent_, true);
	}

}
#endif
