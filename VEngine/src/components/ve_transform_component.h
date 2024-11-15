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

		glm::vec3 GetWorldPosition() { return worldPosition; }
		glm::vec3 GetWorldScale() { return worldScale; }
		glm::vec3 GetWorldRotation() { return worldRotation; }

		glm::mat4 GetTransformMatrix();
		glm::mat4 GetWorldTransformMatrix();
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	private:
		void ApplyChildren(Entity* entity, glm::mat4 parentTransform);

		//Only use these if the entity is a child of another.
		glm::vec3 worldPosition;
		glm::vec3 worldRotation;
		glm::vec3 worldScale;

		friend class SpriteComponent;
		friend class Editor;
	};
}