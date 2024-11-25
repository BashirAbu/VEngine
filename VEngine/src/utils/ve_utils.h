#pragma once
#include "ve_defines.h"
#include <raylib.h>
namespace VE 
{
	VE_API Color GLMVec4ToRayColor(glm::vec4 color);

	VE_API std::filesystem::path GetRelativePath(std::filesystem::path path);
	VE_API std::filesystem::path GetFullPath(std::filesystem::path path);
}