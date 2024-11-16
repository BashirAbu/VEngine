#pragma once
#include "ve_defines.h"
#include <nlohmann/json.hpp>
namespace VE 
{
	class Entity;
	class VE_API Component 
	{
	public:
		Component(Entity* entity);
		virtual ~Component() {}

		virtual void Start() = 0;
		virtual void PreUpdate(float deltaTime) {}
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual void Serialize(nlohmann::json& json) {}
		virtual void Deserialize(nlohmann::json& json) {}
		virtual void DrawEditorUI() {}
	protected:
		//Owner
		Entity* entity;
	};
}