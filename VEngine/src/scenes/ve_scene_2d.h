#pragma once
#include "ve_defines.h"
#include "ve_scene.h"

namespace VE 
{
	class VE_API Scene2D : public Scene 
	{
	public:
		Scene2D();
		virtual ~Scene2D();
		virtual void Render() override;
		virtual void SetMainCamera(class Entity* camera) override;
	private:
		glm::vec4 clearColor;
		Camera2D editorCamera;
		glm::vec2 oldRenderTargetSize;
		class Camera2DEntity* mainCamera = nullptr;
		friend class Editor;
		friend class SceneManager;
	};
}