#include "ve_transform_component.h"
#include "utils/ve_serialization.h"
#include <iostream>
#include "editor/ve_editor_elements.h"
namespace VE 
{
	TransformComponent::TransformComponent(Entity* entity) : Component(entity), position(0.0f), rotation(0.0f), scale(1.0f)
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
	}
	void TransformComponent::Serialize(nlohmann::json& json)
	{
		json["transform_component"]["position"] = Serialize::Vec3(position);
		json["transform_component"]["rotation"] = Serialize::Vec3(rotation);
		json["transform_component"]["scale"] = Serialize::Vec3(scale);
	}
	void TransformComponent::Deserialize(nlohmann::json& json)
	{
		position = Deserialze::Vec3(json["transform_component"]["position"]);
		rotation = Deserialze::Vec3(json["transform_component"]["rotation"]);
		scale = Deserialze::Vec3(json["transform_component"]["scale"]);
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
}