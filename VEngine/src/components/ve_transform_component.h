#pragma once
#include "ve_defines.h"
#include "ve_component.h"
#include <nlohmann/json.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace VE 
{
	class Entity;
	class VE_API TransformComponent : public Component 
	{
	public:
		TransformComponent(Entity* entity);
		~TransformComponent();
		virtual void Start() override;
		virtual void PreUpdate(float deltaTime) override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Serialize(nlohmann::json& json) override;
		virtual void Deserialize(nlohmann::json& json) override;
		virtual void DrawEditorUI() override;

		void Translate(glm::vec3);
		void SetPosition(glm::vec3 vec);
		glm::vec3 GetPosition() { return worldPosition; }

		void SetRotation(glm::vec3 vec);
		glm::vec3 GetRotation() { return worldRotation; }

		void SetScale(glm::vec3 vec);
		glm::vec3 GetScale() { return worldScale; }

		void SetLocalPosition(glm::vec3 vec) { position = vec; }
		glm::vec3 GetLocalPosition() { return position; }

		void SetLocalRotation(glm::vec3 vec) { rotation = vec; }
		glm::vec3 GetLocalRotation() { return rotation; }

		void SetLocalScale(glm::vec3 vec) { scale = vec; }
		glm::vec3 GetLocalScale() { return scale; }

		glm::mat4 GetTransformMatrix();
		glm::mat4 GetWorldTransformMatrix();
	private:
		void ApplyParentTransform(Entity* entity, glm::mat4 parentTransform);
		void UpdateWolrdTransform();

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::vec3 worldPosition;
		glm::vec3 worldRotation;
		glm::vec3 worldScale;

		friend class SpriteComponent;
		friend class Editor;
	};
}