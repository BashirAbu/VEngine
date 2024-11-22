#include "editor/ve_editor.h"
#include "ve_engine.h"
#include "components/ve_components.h"
#include "editor/ve_editor_elements.h"
namespace VE 
{
	void Editor::DrawComponentElements(std::string name, flecs::entity entity) 
	{
		
		if (name == "TransformComponent")
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
			if (open)
			{
				Components::TransformComponent* tc = entity.get_mut<Components::TransformComponent>();
				EditorElement::Vec3(tc->localPosition, "Position");

				EditorElement::Vec3(tc->localRotation, "Rotation");

				EditorElement::Vec3(tc->localScale, "Scale");
				ImGui::TreePop();
			}
			
		}
	}
	
}