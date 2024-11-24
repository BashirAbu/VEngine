#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include "components/ve_components.h"
namespace VE 
{
	Color GLMVec4ToRayColor(glm::vec4 color);

	glm::mat4 GetLocalTransformMatrix(const Components::TransformComponent& tc);
	glm::mat4 GetWorldTransformMatrix(const Components::TransformComponent& tc);
}