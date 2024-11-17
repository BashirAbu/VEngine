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
		virtual const Entity* GetMainCamera() const override;
	private:

		friend class Editor;
		friend class SceneManager;
		friend class Camera2DEntity;

	};
}