#include "ve_sprite_component.h"
#include "ve_transform_component.h"
#include "entities/ve_entity.h"
#include "ve_assets_manager.h"
#include "utils/ve_serialization.h"
#include "editor/ve_editor_elements.h"
#include "platform/ve_platform.h"
#include "ve_engine.h"
namespace VE 
{
	SpriteComponent::SpriteComponent(Entity* entity) : Component(entity), origin(0.0f), tintColor(1.0f), texture(nullptr)
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
		if (texture)
		{
			Rectangle src, dest;
			src.x = 0.0f;
			src.width = (float)texture->width;
			src.y = 0.0f;
			src.height = (float)texture->height;
			if (transformComponent->worldScale.x < 0.0f)
			{
				src.width *= -1.0f;
			}
			if (transformComponent->worldScale.y < 0.0f)
			{
				src.height *= -1.0f;
			}

			dest.x = transformComponent->worldPosition.x;
			dest.y = transformComponent->worldPosition.y;
			dest.width = glm::abs(texture->width * transformComponent->worldScale.x);
			dest.height = glm::abs(texture->height * transformComponent->worldScale.y);

			Vector2 org = { dest.width * origin.x, dest.height * origin.y };
			Color col = { uint8_t(tintColor.x * 255.0f), uint8_t(tintColor.y * 255.0f), uint8_t(tintColor.z * 255.0f), uint8_t(tintColor.w * 255.0f) };
			DrawTexturePro(*texture, src, dest, org, transformComponent->worldRotation.z, col);
		}
	}

	void SpriteComponent::Serialize(nlohmann::json& json)
	{
		json["sprite_component"]["origin"] = Serialize::Vec2(origin);
		json["sprite_component"]["tint_color"] = Serialize::Vec4(tintColor);
		json["sprite_component"]["texture_path"] = texturePath.string();
	}
	void SpriteComponent::Deserialize(nlohmann::json& json)
	{
		if (json.contains("sprite_component"))
		{
			origin = Deserialze::Vec2(json["sprite_component"]["origin"]);
			tintColor = Deserialze::Vec4(json["sprite_component"]["tint_color"]);
			if (json["sprite_component"].contains("texture_path"))
			{
				texturePath = (std::string)json["sprite_component"]["texture_path"];
				LoadTexture(texturePath);
			}
		}
	}
	void SpriteComponent::DrawEditorUI()
	{
		bool opened = ImGui::TreeNodeEx((void*)((uint64_t)this), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Component");
		if (opened)
		{
			ImGui::Text("Texture: %s", texturePath.filename().string().c_str());
			ImGui::SameLine();
			if (ImGui::Button("Load Texture"))
			{
				texturePath = VE::OpenFileDialog();
				if (!texturePath.empty())
				{
					std::filesystem::path relativePath = texturePath.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().string() + "/assets");
					if (!relativePath.empty())
					{
						LoadTexture(relativePath.generic_string());
					}
				}
			}
			ImGui::Spacing();
			EditorElement::Vec2(origin, "Origin");
			ImGui::Spacing();
			EditorElement::Color(tintColor, "Tint Color");
			ImGui::TreePop();
		}
		
	}
	void SpriteComponent::LoadTexture(std::filesystem::path path)
	{
		texturePath = path;
		texture = AssetsManager::GetSingleton()->LoadTexture(path);
	}
}
