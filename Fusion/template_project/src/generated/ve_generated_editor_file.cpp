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


extern "C" VE_PROJECT_API void ProjectDrawComponentElements(std::string name, flecs::entity entity)
{
	ImGui::PushID(name.c_str());
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	ImGui::PopID();
}
#endif
