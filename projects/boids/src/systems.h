#pragma once
#include <ve_defines.h>
#include <components/ve_components.h>
#include "components.h"

VE_FUNCTION(System, Update)
void MovementSystem(flecs::entity e, VE::Components::TransformComponent& tc,
	BoidComponent& bc);
VE_FUNCTION(System, PreFrame)
void ManagerSystem(flecs::entity e,
	ManagerComponent& mc);
