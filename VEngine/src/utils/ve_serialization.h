#pragma once
#include "ve_defines.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
namespace VE 
{
	namespace Serialize 
	{
		VE_API nlohmann::json Vec2(glm::vec2 vec);
		VE_API nlohmann::json Vec3(glm::vec3 vec);
		VE_API nlohmann::json Vec4(glm::vec4 vec);
	}
	namespace Deserialze 
	{
		VE_API glm::vec2 Vec2(nlohmann::json& json);
		VE_API glm::vec3 Vec3(nlohmann::json& json);
		VE_API glm::vec4 Vec4(nlohmann::json& json);
	}
}