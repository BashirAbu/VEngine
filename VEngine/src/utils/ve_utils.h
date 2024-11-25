#pragma once
#include "ve_defines.h"
#include <raylib.h>
namespace VE 
{
	Color GLMVec4ToRayColor(glm::vec4 color);

	glm::mat4 GetLocalTransformMatrix(flecs::entity e);
	glm::mat4 GetWorldTransformMatrix(flecs::entity e);

	std::filesystem::path GetRelativePath(std::filesystem::path path);
	std::filesystem::path GetFullPath(std::filesystem::path path);
}