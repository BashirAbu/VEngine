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
			Components::TransformComponent* tc = entity.get_mut<Components::TransformComponent>();
			glm::vec3 p = tc->localPosition;
			EditorElement::Vec3(p, "Position");
			tc->localPosition = p;

			glm::vec3 r = tc->localRotation;
			EditorElement::Vec3(r, "Rotation");
			tc->localRotation = r;

			glm::vec3 s = tc->localScale;
			EditorElement::Vec3(s, "Scale");
			tc->localScale = s;
			
		}
	}
	
}