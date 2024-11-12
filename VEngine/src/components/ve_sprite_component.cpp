#include "ve_sprite_component.h"
#include "ve_transform_component.h"
#include "entities/ve_entity.h"
#include "ve_assets_manager.h"
#include "utils/ve_serialization.h"
#include "editor/ve_editor_elements.h"
namespace VE 
{
	SpriteComponent::SpriteComponent(Entity* entity) : Component(entity), origin(0.0f), tintColor(1.0f)
	{
		transformComponent = entity->GetComponent<TransformComponent>();
	}
	SpriteComponent::~SpriteComponent()
	{

	}
	void SpriteComponent::Start()
	{
	}
	void SpriteComponent::Update(float deltaTime)
	{
	}
	void SpriteComponent::Render()
	{
		Rectangle src, dest;
		src.x = 0.0f;
		src.width = (float)texture->width;
		src.y = 0.0f;
		src.height = (float)texture->height;
		if (transformComponent->scale.x < 0.0f)
		{
			src.width *= -1.0f;
		}
		if (transformComponent->scale.y < 0.0f)
		{
			src.height *= -1.0f;
		}

		dest.x = transformComponent->position.x;
		dest.y = transformComponent->position.y;
		dest.width = glm::abs(texture->width * transformComponent->scale.x);
		dest.height = glm::abs(texture->height * transformComponent->scale.y);

		Vector2 org = { dest.width * origin.x, dest.height * origin.y };
		Color col = { uint8_t(tintColor.x * 255.0f), uint8_t(tintColor.y * 255.0f), uint8_t(tintColor.z * 255.0f), uint8_t(tintColor.w * 255.0f) };
		DrawTexturePro(*texture, src, dest, org, transformComponent->rotation.z, col);
	}

	void SpriteComponent::Serialize(nlohmann::json& json)
	{
		json["sprite_component"]["origin"] = Serialize::Vec2(origin);
		json["sprite_component"]["tint_color"] = Serialize::Vec4(tintColor);
		
	}
	void SpriteComponent::Deserialize(nlohmann::json& json)
	{
		origin = Deserialze::Vec2(json["sprite_component"]["origin"]);
		tintColor = Deserialze::Vec4(json["sprite_component"]["tint_color"]);
	}
	void SpriteComponent::DrawEditorUI()
	{
		ImGui::Text("Sprite Component");
		ImGui::Spacing();
		EditorElement::Vec2(origin, "Origin");
		ImGui::Spacing();
		EditorElement::Color(tintColor, "Tint Color");
	}
}
