#pragma once
#include "ve_defines.h"
#include "ve_entity.h"
#include <raylib.h>
namespace VE 
{
	class VE_API Camera2DEntity : public Entity 
	{
	public:
		Camera2DEntity(std::string name);
		~Camera2DEntity();

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Serialize(nlohmann::json& json) override;
		virtual void Deserialize(nlohmann::json& json) override;
		virtual void DrawEditorUI() override;
		const RenderTexture2D* GetRenderTarget() const { return &renderTarget; }
	private:
		Camera2D camera2D = {};
		RenderTexture2D renderTarget;
		int32_t renderTargetWidth;
		int32_t renderTargetHeight;
		bool mainCamera = false;
		friend class Scene;
		friend class Scene2D;
		friend class Engine;
	};
}