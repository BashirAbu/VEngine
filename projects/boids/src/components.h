#pragma once
#include <ve_defines.h>
#include <glm/glm.hpp>
#include <scenes/ve_scene.h>
#include <components/ve_components.h>
VE_CLASS(Component)
struct ManagerComponent
{
	VE_PROPERTY(Editor)
		float speed = 0.0f;
	VE_PROPERTY(Editor)
		float radius = 0.0f;
	VE_PROPERTY(Editor)
		float alignment = 0.0f;
	VE_PROPERTY(Editor)
		float seperation = 0.0f;
	VE_PROPERTY(Editor)
		float cohesion = 0.0f;

};


VE_CLASS(Component, OnAdd = OnBoidComponentAdd)
struct BoidComponent
{
	glm::vec3 velocity = {};
	glm::vec3 acceleration = {};
	glm::vec3 direction = {};
};


VE_FUNCTION(Callback)
inline void OnBoidComponentAdd(BoidComponent& bc)
{
	srand(time(NULL));
	bc.direction = glm::vec3((float)((rand() % 200) - 100) / 100.0f,
		(float)((rand() % 200) - 100) / 100.0f, 0.0f);
}