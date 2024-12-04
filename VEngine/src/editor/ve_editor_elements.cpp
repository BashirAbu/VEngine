#include "ve_editor_elements.h"
#include "platform/ve_platform.h"
#include "ve_engine.h"
#include "utils/ve_utils.h"
namespace VE
{
	namespace EditorElement
	{
       



        void Vec2(glm::vec2& vec, std::string label)
        {
            ImGui::PushID(label.c_str());
            ImGui::Text(label.c_str());
 
            ImGui::Columns(2, 0, false);
            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##x", &vec.x);
            ImGui::PopItemWidth();
            ImGui::NextColumn();


            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##y", &vec.y);
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Columns(1);


            ImGui::PopID();
            
        }
        void Vec3(glm::vec3& vec, std::string label)
		{

            ImGui::PushID(label.c_str());

            ImGui::Text(label.c_str());

            ImGui::Columns(3, 0, false);

            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##x", &vec.x);
            ImGui::PopItemWidth();
            ImGui::NextColumn();


            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##y", &vec.y);
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##z", &vec.z);
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Columns(1);


            ImGui::PopID();
		}
        void Vec4(glm::vec4& vec, std::string label)
        {
            ImGui::PushID(label.c_str());

            ImGui::Text(label.c_str());

            ImGui::Columns(4, 0, false);

            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##x", &vec.x);
            ImGui::PopItemWidth();
            ImGui::NextColumn();


            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##y", &vec.y);
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##z", &vec.z);
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Text("W");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##w", &vec.w);
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Columns(1);


            ImGui::PopID();
        }
        void Color(glm::vec4& color, std::string label)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::ColorPicker4(label.c_str(), &color.x);
            ImGui::NextColumn();

            ImGui::Columns(1);
        }
        VE_API void Float(float& variable, std::string label)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragFloat(((std::string)"##" + label).c_str(), &variable);
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Double(double& variable, std::string label) 
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragFloat(((std::string)"##" + label).c_str(), (float*)&variable);
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Int(uint8_t& variable, std::string label) 
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT8_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);
            
        }
        VE_API void Int(uint16_t& variable, std::string label) 
        {

            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT16_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);
        }
        VE_API void Int(uint32_t& variable, std::string label) 
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT32_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Int(uint64_t& variable, std::string label) 
        {

            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT32_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);            
        }
        VE_API void Int(int8_t& variable, std::string label) 
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT8_MIN, INT8_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);
        }
        VE_API void Int(int16_t& variable, std::string label) 
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT16_MIN, INT16_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Int(int32_t& variable, std::string label) 
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT32_MIN, INT32_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);
        }
        VE_API void Int(int64_t& variable, std::string label) 
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT32_MIN, INT32_MAX);
            ImGui::NextColumn();
            ImGui::Columns(1);

        }

        VE_API void String(std::string& variable, std::string label) 
        {

            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();

            char buffer[255];
            strcpy(buffer, variable.c_str());
            ImGui::InputText(((std::string)"##" + label).c_str(), buffer, sizeof(buffer));
            if (variable.capacity() < sizeof(buffer))
            {
                variable.resize(sizeof(buffer));
            }
            strcpy(variable.data(), buffer);;
            ImGui::NextColumn();
            ImGui::Columns(1);
            
        }

        VE_API void FileSystem(std::filesystem::path& path, std::string label)
        {

            ImGui::Columns(2, 0, false);
            ImGui::Text((label + ": %s").c_str(), path.generic_string().c_str());
            ImGui::NextColumn();
            if (ImGui::Button("Browse"))
            {
                path = VE::OpenFileDialog();
                if (!path.empty())
                {
                    path = GetRelativePath(path);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);

        }

        VE_API void Checkbox(bool& variable, std::string label)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ImGui::Checkbox(((std::string)"##" + label.c_str()).c_str(), &variable);
            ImGui::NextColumn();
            ImGui::Columns(1);
           
        }

    }
}