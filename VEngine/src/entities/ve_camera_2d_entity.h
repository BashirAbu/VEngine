#pragma once
#include "ve_defines.h"
#include "ve_camera_entity.h"
#include <raylib.h>
namespace VE 
{
	class VE_API Camera2DEntity : public CameraEntity 
	{
	public:
		Camera2DEntity(std::string name = "");
		~Camera2DEntity();

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Serialize(nlohmann::json& json) override;
		virtual void Deserialize(nlohmann::json& json) override;
		virtual void DrawEditorUI() override;
	private:
		Camera2D camera2D = {};

		friend class Editor;
		friend class Scene;

	};
}