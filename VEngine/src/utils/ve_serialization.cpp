#include "ve_serialization.h"

namespace VE 
{
	namespace Serialize 
	{
		nlohmann::json Vec2(glm::vec2 vec)
		{
			return {
				{"x", vec.x},
				{"y", vec.y}
			};
		}
		nlohmann::json Vec3(glm::vec3 vec)
		{
			
			return {
				{"x", vec.x},
				{"y", vec.y},
				{"z", vec.z}
			};
		}
		nlohmann::json Vec4(glm::vec4 vec)
		{
			
			return {
				{"x", vec.x},
				{"y", vec.y},
				{"z", vec.z},
				{"w", vec.w}
			};
		}
	}

	namespace Deserialze
	{
		glm::vec2 Vec2(nlohmann::json& json)
		{
			glm::vec2 vec;

			vec.x = json.contains("x") ? (float)json["x"] : 0.0f;
			vec.y = json.contains("y")? (float)json["y"] : 0.0f;

			return vec;
		}
		glm::vec3 Vec3(nlohmann::json& json)
		{
			glm::vec3 vec;

			vec.x = json.contains("x") ? (float)json["x"] : 0.0f;
			vec.y = json.contains("y") ? (float)json["y"] : 0.0f;
			vec.z = json.contains("z") ? (float)json["z"] : 0.0f;

			return vec;
		}
		glm::vec4 Vec4(nlohmann::json& json)
		{
			glm::vec4 vec;

			vec.x = json.contains("x") ? (float)json["x"] : 0.0f;
			vec.y = json.contains("y") ? (float)json["y"] : 0.0f;
			vec.z = json.contains("z") ? (float)json["z"] : 0.0f;
			vec.w = json.contains("w") ? (float)json["w"] : 0.0f;

			return vec;
		}
	}
}