#pragma once
#include "ve_defines.h"
#include "ve_entity.h"
#include <raylib.h>
namespace VE 
{
	class VE_API CameraEntity : public Entity 
	{
	public:
		CameraEntity(std::string name = "");
		virtual ~CameraEntity();

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Serialize(nlohmann::json& json) override;
		virtual void Deserialize(nlohmann::json& json) override;
		virtual void DrawEditorUI() override;
		const RenderTexture* GetRenderTarget() const { return &renderTarget; }
	private:
		RenderTexture renderTarget;
		int32_t renderTargetWidth;
		int32_t renderTargetHeight;
		bool mainCamera = false;
		friend class Scene;
		friend class Scene2D;
		friend class Engine;
		friend class Editor;
	};
}