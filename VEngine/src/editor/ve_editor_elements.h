#pragma once
#ifdef VE_EDITOR
#include "ve_defines.h"
#include <imgui.h>
namespace VE 
{

	namespace EditorElement 
	{
		VE_API void Vec2(glm::vec2& vec, std::string label);
		VE_API void Vec3(glm::vec3& vec, std::string label);
		VE_API void Vec4(glm::vec4& vec, std::string label);
		VE_API void Color(glm::vec4& color, std::string label);
		VE_API void Float(float& variable, std::string label);
		VE_API void Double(double& variable, std::string label);
		VE_API void Int(uint8_t& variable, std::string label);
		VE_API void Int(uint16_t& variable, std::string label);
		VE_API void Int(uint32_t& variable, std::string label);
		VE_API void Int(uint64_t& variable, std::string label);
		VE_API void Int(int8_t& variable, std::string label);
		VE_API void Int(int16_t& variable, std::string label);
		VE_API void Int(int32_t& variable, std::string label);
		VE_API void Int(int64_t& variable, std::string label);
		VE_API void String(std::string& variable, std::string label);
		VE_API void FileSystem(std::filesystem::path& path, std::string label, void* data = nullptr, std::function<void(void* data)> callback = nullptr);
		VE_API void Checkbox(bool& variable, std::string label, void* data = nullptr, std::function<void(void* data)> callback = nullptr);
		VE_API void Combo(const char** items, int* currentItem, size_t items_count, std::string label, void* data = nullptr, std::function<void(void* data)> callback = nullptr);
	}

}

#endif