#include "ve_camrea_entity.h"
#include "ve_engine.h"

namespace VE 
{
	CameraEntity::CameraEntity(std::string name) : Entity(name)
	{
		if (Engine::GetSingleton()->GetSceneManager()->GetCurrentScene())
		{
			Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->cameras.push_back(this);
			if (Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->cameras.size() == 1)
			{
				Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->SetMainCamera(this);
			}
		}

		renderTargetWidth = Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth;
		renderTargetHeight = Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight;
		renderTarget = LoadRenderTexture(renderTargetWidth, renderTargetHeight, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		SetTextureFilter(renderTarget.texture, TEXTURE_FILTER_BILINEAR);
		SetTextureWrap(renderTarget.texture, TEXTURE_WRAP_CLAMP);
	}
	CameraEntity::~CameraEntity()
	{
		UnloadRenderTexture(renderTarget);
		for (auto itr = Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->cameras.begin();
			itr != Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->cameras.end();
			itr++)
		{
			if (*itr == this)
			{
				if (Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->mainCamera == this)
				{

					Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->mainCamera = nullptr;

				}
				Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->cameras.erase(itr);
				break;
			}
		}
	}
	void CameraEntity::Start()
	{
	}
	void CameraEntity::Update(float deltaTime)
	{
	}
	void CameraEntity::Render()
	{
	}
	void CameraEntity::Serialize(nlohmann::json& json)
	{
	}
	void CameraEntity::Deserialize(nlohmann::json& json)
	{
	}
	void CameraEntity::DrawEditorUI()
	{
	}
}