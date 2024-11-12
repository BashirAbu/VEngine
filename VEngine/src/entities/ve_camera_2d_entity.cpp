#include "ve_camera_2d_entity.h"
#include "ve_engine.h"
#include "editor/ve_editor_elements.h"
namespace VE 
{
	Camera2DEntity::Camera2DEntity(std::string name) : Entity(name)
	{
		if (Engine::GetSingleton()->GetSceneManager()->currentScene)
		{
			Engine::GetSingleton()->GetSceneManager()->currentScene->cameras.push_back(this);
			if (Engine::GetSingleton()->GetSceneManager()->currentScene->cameras.size() == 1)
			{
				Engine::GetSingleton()->GetSceneManager()->currentScene->SetMainCamera(this);
			}
		}

		renderTargetWidth = Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth;
		renderTargetHeight = Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight;
		renderTarget = LoadRenderTexture(renderTargetWidth, renderTargetHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		SetTextureFilter(renderTarget.texture, TEXTURE_FILTER_BILINEAR);

		camera2D = {};
		camera2D.zoom = 1.0f;
	}
	Camera2DEntity::~Camera2DEntity()
	{
		UnloadRenderTexture(renderTarget);
		for (auto itr = Engine::GetSingleton()->GetSceneManager()->currentScene->cameras.begin();
			itr != Engine::GetSingleton()->GetSceneManager()->currentScene->cameras.end();
			itr++)
		{
			if (*itr == this)
			{
				Engine::GetSingleton()->GetSceneManager()->currentScene->cameras.erase(itr);
				break;
			}
		}
	}
	void Camera2DEntity::Start()
	{
	}
	void Camera2DEntity::Update(float deltaTime)
	{
	}
	void Camera2DEntity::Render()
	{
		camera2D.target = Vector2{ transformComponent->position.x, transformComponent->position.y };
		camera2D.rotation = transformComponent->rotation.z;
	}
	void Camera2DEntity::Serialize(nlohmann::json& json)
	{
	}
	void Camera2DEntity::Deserialize(nlohmann::json& json)
	{
	}
	void Camera2DEntity::DrawEditorUI()
	{
		ImGui::Text("Camera 2D");
		ImGui::Spacing();
		ImGui::DragFloat("Zoom", &camera2D.zoom);
		ImGui::Spacing();
		glm::vec2 offset = {camera2D.offset.x, camera2D.offset.y};
		EditorElement::Vec2(offset, "Offset");
		camera2D.offset = Vector2{offset.x, offset.y};
	}
}