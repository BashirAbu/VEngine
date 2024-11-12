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

			vec.x = json["x"];
			vec.y = json["y"];

			return vec;
		}
		glm::vec3 Vec3(nlohmann::json& json)
		{
			glm::vec3 vec;

			vec.x = json["x"];
			vec.y = json["y"];
			vec.z = json["z"];

			return vec;
		}
		glm::vec4 Vec4(nlohmann::json& json)
		{
			glm::vec4 vec;

			vec.x = json["x"];
			vec.y = json["y"];
			vec.z = json["z"];
			vec.w = json["w"];

			return vec;
		}
	}
}