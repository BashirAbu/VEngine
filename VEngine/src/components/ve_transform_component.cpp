#include "ve_transform_component.h"
#include "utils/ve_serialization.h"
#include <iostream>
#include "editor/ve_editor_elements.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "entities/ve_entity.h"
namespace VE 
{
	TransformComponent::TransformComponent(Entity* entity) : Component(entity), position(0.0f), rotation(0.0f), scale(1.0f), worldPosition(0.0f), worldRotation(0.0f), worldScale(0.0f)
	{
	}
	TransformComponent::~TransformComponent()
	{
	}
	void TransformComponent::Start()
	{
	}
	void TransformComponent::Update(float deltaTime)
	{
	}
	void TransformComponent::Render()
	{
		if (entity->GetParent())
		{
			glm::mat4 parentTransform = entity->GetParent()->transformComponent->GetTransformMatrix();
			glm::mat4 childTransform = GetTransformMatrix();

			glm::mat4 worldTransform = parentTransform * childTransform;
			glm::quat rot;
			glm::vec3 skew;
			glm::vec4 pres;
			glm::decompose(worldTransform, worldScale, rot, worldPosition, skew, pres);

			worldRotation = glm::degrees(glm::eulerAngles(rot));
		}
		else 
		{
			worldPosition = position;
			worldScale = scale;
			worldRotation = rotation;
		}
		
	}
	void TransformComponent::Serialize(nlohmann::json& json)
	{
		json["transform_component"]["position"] = Serialize::Vec3(position);
		json["transform_component"]["rotation"] = Serialize::Vec3(rotation);
		json["transform_component"]["scale"] = Serialize::Vec3(scale);
	}
	void TransformComponent::Deserialize(nlohmann::json& json)
	{
		if (json.contains("transform_component"))
		{
			position = Deserialze::Vec3(json["transform_component"]["position"]);
			rotation = Deserialze::Vec3(json["transform_component"]["rotation"]);
			scale = Deserialze::Vec3(json["transform_component"]["scale"]);
		}
	}
	void TransformComponent::DrawEditorUI()
	{
		bool opened = ImGui::TreeNodeEx((void*)((uint64_t)this), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component");
		if (opened)
		{
			ImGui::Spacing();
			EditorElement::Vec3(position, "Position");
			ImGui::Spacing();
			EditorElement::Vec3(rotation, "Rotation");
			ImGui::Spacing();
			EditorElement::Vec3(scale,    "Scale   ");
			ImGui::TreePop();
		}
	}
	glm::mat4 TransformComponent::GetTransformMatrix()
	{
		glm::mat4 transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::scale(glm::mat4(1.0f), scale);
		return transformMatrix;
	}
}