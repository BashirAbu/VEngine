#pragma once
#include "ve_defines.h"
#include "ve_component.h"
#include <raylib.h>
#include <glm/glm.hpp>
namespace VE 
{
	class TransformComponent;
	class VE_API SpriteComponent : public Component
	{
	public:
		SpriteComponent(Entity* entity);
		~SpriteComponent();

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Serialize(nlohmann::json& json) override;
		virtual void Deserialize(nlohmann::json& json) override;
		virtual void DrawEditorUI() override;
		Texture GetTexture() { return *texture; }
		void LoadTexture(std::filesystem::path path);


		glm::vec2 origin;
		glm::vec4 tintColor;
	private:
		Texture* texture;
		std::filesystem::path texturePath;
		TransformComponent* transformComponent;
	};
}