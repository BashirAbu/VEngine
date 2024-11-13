#pragma once
#include "ve_defines.h"
#include "ve_component.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace VE 
{
	class Entity;
	class VE_API TransformComponent : public Component 
	{
	public:
		TransformComponent(Entity* entity);
		~TransformComponent();
		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Serialize(nlohmann::json& json) override;
		virtual void Deserialize(nlohmann::json& json) override;
		virtual void DrawEditorUI() override;

		glm::mat4 GetTransformMatrix();

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	private:
	};
}