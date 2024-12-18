#ifdef VE_EDITOR
#include "ve_editor_elements.h"
#include "platform/ve_platform.h"
#include "ve_engine.h"
#include "utils/ve_utils.h"
#include <rlImGui.h>
namespace VE
{
	namespace EditorElement
	{
       
        void Vec2(glm::vec2& vec, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::PushID(label.c_str());
            ImGui::Text(label.c_str());
 
            ImGui::Columns(2, 0, false);
            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##x", &vec.x)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();


            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##y", &vec.y)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Columns(1);


            ImGui::PopID();
            
        }
        void Vec3(glm::vec3& vec, std::string label, void* data, std::function<void(void* data)> callback)
		{

            ImGui::PushID(label.c_str());

            ImGui::Text(label.c_str());

            ImGui::Columns(3, 0, false);

            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##x", &vec.x)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();


            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##y", &vec.y)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##z", &vec.z)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Columns(1);


            ImGui::PopID();
		}
        void Vec4(glm::vec4& vec, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::PushID(label.c_str());

            ImGui::Text(label.c_str());

            ImGui::Columns(4, 0, false);

            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##x", &vec.x)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();


            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##y", &vec.y)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##z", &vec.z)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Text("W");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::DragFloat("##w", &vec.w)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::PopItemWidth();
            ImGui::NextColumn();

            ImGui::Columns(1);


            ImGui::PopID();
        }
        void Color(glm::vec4& color, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::ColorPicker4(label.c_str(), &color.x)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();

            ImGui::Columns(1);
        }
        VE_API void Float(float& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragFloat(((std::string)"##" + label).c_str(), &variable)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Double(double& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragFloat(((std::string)"##" + label).c_str(), (float*)&variable)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Int(uint8_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT8_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);
            
        }
        VE_API void Int(uint16_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {

            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT16_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);
        }
        VE_API void Int(uint32_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT32_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Int(uint64_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {

            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, 0, UINT32_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);            
        }
        VE_API void Int(int8_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT8_MIN, INT8_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);
        }
        VE_API void Int(int16_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT16_MIN, INT16_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);

        }
        VE_API void Int(int32_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT32_MIN, INT32_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);
        }
        VE_API void Int(int64_t& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::DragInt(((std::string)"##" + label).c_str(), (int*)&variable, 1.0f, INT32_MIN, INT32_MAX)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);

        }

        VE_API void String(std::string& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {

            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();

           
            if (ImGui::InputText(((std::string)"##" + label).c_str(), variable.data(), variable.size() + 1,
                ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) -> int
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                    {
                        auto* str = static_cast<std::string*>(data->UserData);
                        str->resize(data->BufTextLen);
                        data->Buf = str->data();
                    }
                    return 0;
                }, &variable))

            {
                if (callback)
                {
                    callback(data);
                }
            }


            ImGui::NextColumn();
            ImGui::Columns(1);
            
        }


        VE_API void FileSystem(std::filesystem::path& path, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::PushID(path.c_str());
            ImGui::Columns(2, 0, false);
            ImGui::Text((label + ": %s").c_str(), path.generic_string().c_str());
            ImGui::NextColumn();
            if (ImGui::Button("Browse"))
            {
                path = VE::OpenFileDialog();
                if (!path.empty())
                {
                    path = GetRelativePath(path);
                    if (callback)
                    {
                        callback(data);
                    }
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::PopID();
        }

        VE_API void Checkbox(bool& variable, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::Checkbox(((std::string)"##" + label.c_str()).c_str(), &variable)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);
           
        }

        VE_API void Combo(const char** items, int* currentItem, size_t items_count, std::string label, void* data, std::function<void(void* data)> callback)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (ImGui::Combo(("##" + label).c_str(), currentItem, items, (int)items_count)) 
            {
                if (callback)
                {
                    callback(data);
                }
            }
            ImGui::NextColumn();
            ImGui::Columns(1);
        }

        VE_API void Image(const Texture* texture, std::string label)
        {
            ImGui::Columns(2, 0, false);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            if (texture->id > 0)
            {
                rlImGuiImageSize(texture, 100, 100);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
        }

    }
}
#endif