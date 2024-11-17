#include "ve_camera_2d_entity.h"
#include "ve_engine.h"
#include "editor/ve_editor_elements.h"
namespace VE 
{
	Camera2DEntity::Camera2DEntity(std::string name) : CameraEntity(name)
	{
		camera2D = {};
		camera2D.zoom = 1.0f;
	}
	Camera2DEntity::~Camera2DEntity()
	{
		
	}
	void Camera2DEntity::Start()
	{
	}
	void Camera2DEntity::Update(float deltaTime)
	{
	}
	void Camera2DEntity::Render()
	{
		camera2D.target = Vector2{ transformComponent->GetPosition().x,transformComponent->GetPosition().y};
		camera2D.rotation = transformComponent->GetRotation().z;
	}
	void Camera2DEntity::Serialize(nlohmann::json& json)
	{
	}
	void Camera2DEntity::Deserialize(nlohmann::json& json)
	{
	}
	void Camera2DEntity::DrawEditorUI()
	{
		
		bool opened = ImGui::TreeNodeEx((void*)((uint64_t)this), ImGuiTreeNodeFlags_DefaultOpen, "Camera 2D");
		if (opened)
		{
			ImGui::Spacing();
			ImGui::DragFloat("Zoom", &camera2D.zoom);
			ImGui::Spacing();
			glm::vec2 offset = { camera2D.offset.x, camera2D.offset.y };
			EditorElement::Vec2(offset, "Offset");
			camera2D.offset = Vector2{ offset.x, offset.y };
			ImGui::TreePop();
		}
		
	}
}