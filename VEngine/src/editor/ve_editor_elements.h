#pragma once
#include "ve_defines.h"
#include <imgui.h>
namespace VE 
{

	namespace EditorElement 
	{
		VE_API void Vec2(glm::vec2& vec, std::string label);
		VE_API void Vec3(glm::vec3& vec, std::string label);
		VE_API void Color(glm::vec4& color, std::string label);
	}

}

