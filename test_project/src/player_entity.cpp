#include "player_entity.h"
#include <raylib.h>
#include "ve_engine.h"
#include "components/ve_sprite_component.h"
#include "ve_assets_manager.h"
#include "ve_input.h"
PlayerEntity::PlayerEntity(std::string name) : Entity(name)
{
	VE::SpriteComponent* sc = AddComponent<VE::SpriteComponent>();
	sc->texture = VE::AssetsManager::GetSingleton()->LoadTexture("ape.png");
	
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::Start()
{
}

void PlayerEntity::Update(float deltaTime)
{
	if (VE::Input::IsKeyDown(KEY_D))
	{
		transformComponent->position.x += 100.0f * deltaTime;
	}
	if (VE::Input::IsKeyDown(KEY_A))
	{
		transformComponent->position.x -= 100.0f * deltaTime;
	}
}
