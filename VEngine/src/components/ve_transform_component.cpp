#include "ve_transform_component.h"
#include "utils/ve_serialization.h"
#include <iostream>
#include "editor/ve_editor_elements.h"
#include "entities/ve_entity.h"
#include "ve_engine.h"
namespace VE 
{
	TransformComponent::TransformComponent(Entity* entity) : Component(entity), position(0.0f), rotation(0.0f), scale(1.0f), worldPosition(0.0f), worldRotation(0.0f), worldScale(0.0f)
	{
		worldPosition = position;
		worldRotation = rotation;
		worldScale = scale;
	}
	TransformComponent::~TransformComponent()
	{
	}
	void TransformComponent::Start()
	{
	}
	void TransformComponent::PreUpdate(float deltaTime)
	{
		UpdateWolrdTransform();
	}
	void TransformComponent::Update(float deltaTime)
	{

	}

	void TransformComponent::ApplyParentTransform(Entity* entity, glm::mat4 parentTransform)
	{
		for (auto* child : entity->GetChildren())
		{
			glm::mat4 childTransform = child->transformComponent->GetTransformMatrix();

			glm::mat4 worldTransform = parentTransform * childTransform;
			glm::quat rot;
			glm::vec3 skew;
			glm::vec4 pres;
			glm::decompose(worldTransform, child->transformComponent->worldScale, rot, child->transformComponent->worldPosition, skew, pres);

			child->transformComponent->worldRotation = glm::degrees(glm::eulerAngles(rot));
			if (child->GetChildren().size() > 0)
			{
				ApplyParentTransform(child,child->transformComponent->GetWorldTransformMatrix());
			}
		}
	}

	void TransformComponent::UpdateWolrdTransform()
	{
		if (entity->GetChildren().size() > 0 && !entity->GetParent())
		{
			worldPosition = position;
			worldRotation = rotation;
			worldScale = scale;
			ApplyParentTransform(entity, GetTransformMatrix());
		}
		else if (entity->GetChildren().size() == 0 && !entity->GetParent())
		{
			worldPosition = position;
			worldRotation = rotation;
			worldScale = scale;
		}
	}

	void TransformComponent::Render()
	{
		if (Engine::GetSingleton()->GetSceneManager()->GetSceneMode() == SceneMode::Editor)
		{
			UpdateWolrdTransform();
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

	void TransformComponent::Translate(glm::vec3 vec)
	{
		SetPosition(GetPosition() + vec);
	}

	void TransformComponent::SetPosition(glm::vec3 vec)
	{
		if (entity->GetParent())
		{

			glm::vec4 localPos = glm::inverse(entity->GetParent()->transformComponent->GetWorldTransformMatrix()) * glm::vec4(vec, 1.0f);

			position = localPos;
		}
		else 
		{
			position = vec;
		}

		UpdateWolrdTransform();

	}
	void TransformComponent::SetRotation(glm::vec3 vec)
	{
		if (entity->GetParent())
		{
			glm::mat4 invParentScaleMatrix = glm::inverse(glm::scale(glm::mat4(1.0f), entity->GetParent()->transformComponent->GetScale()));


			glm::quat worldRotQuat = glm::quat(glm::vec3(glm::radians(vec)));

			glm::quat parentWorldRotQuat = glm::quat_cast(glm::mat3(invParentScaleMatrix * entity->GetParent()->transformComponent->GetWorldTransformMatrix()));

			glm::quat localRotQuat = glm::inverse(parentWorldRotQuat) * worldRotQuat;

			glm::vec3 localRotEulerAngles = glm::eulerAngles(localRotQuat);

			rotation = glm::degrees(localRotEulerAngles);
		}
		else 
		{
			rotation = vec;
		}
		UpdateWolrdTransform();
	}
	void TransformComponent::SetScale(glm::vec3 vec)
	{

		if (entity->GetParent())
		{

			glm::vec4 localScl = glm::inverse(entity->GetParent()->transformComponent->GetWorldTransformMatrix()) * glm::vec4(vec, 1.0f);

			scale = localScl;
		}
		else
		{
			scale = vec;
		}

		UpdateWolrdTransform();
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
	glm::mat4 TransformComponent::GetWorldTransformMatrix()
	{
		glm::mat4 transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(glm::mat4(1.0f), worldPosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(worldRotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(worldRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(worldRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::scale(glm::mat4(1.0f), worldScale);
		return transformMatrix;
	}

}