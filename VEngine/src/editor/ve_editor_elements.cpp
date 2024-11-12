#include "ve_editor_elements.h"

namespace VE
{
	namespace EditorElement
	{
        void Vec2(glm::vec2& vec, std::string label)
        {
            ImGui::Text(label.c_str());
            ImGui::SameLine();

            ImGui::PushItemWidth(50);

            ImGui::PushID((label + "x").c_str());
            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::DragFloat("##X", &vec.x, 0.1f);
            ImGui::PopID();


            ImGui::PushID((label + "y").c_str());
            ImGui::SameLine();
            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &vec.y, 0.1f);
            ImGui::PopID();

            ImGui::PopItemWidth();
        }
        void Vec3(glm::vec3& vec, std::string label)
		{

            ImGui::Text(label.c_str());             
            ImGui::SameLine();                      

            ImGui::PushItemWidth(50);    

            ImGui::PushID((label + "x").c_str());
            ImGui::Text("X");                       
            ImGui::SameLine();
            ImGui::DragFloat("##X", &vec.x, 0.1f);  
            ImGui::PopID();


            ImGui::PushID((label + "y").c_str());
            ImGui::SameLine();
            ImGui::Text("Y");                       
            ImGui::SameLine();
            ImGui::DragFloat("##Y", &vec.y, 0.1f);  
            ImGui::PopID();

            ImGui::PushID((label + "z").c_str());
            ImGui::SameLine();
            ImGui::Text("Z");                       
            ImGui::SameLine();
            ImGui::DragFloat("##Z", &vec.z, 0.1f); 
            ImGui::PopID();


            ImGui::PopItemWidth();                  
		}
        void Color(glm::vec4& color, std::string label)
        {
            ImGui::Text(label.c_str());
            ImGui::SameLine();
            ImGui::ColorPicker4(label.c_str(), &color.x);
        }
	}
}