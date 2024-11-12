#include "ve_empty_entity.h"

namespace VE 
{
	EmptyEntity::EmptyEntity(std::string name) : Entity(name)
	{
	}
	EmptyEntity::~EmptyEntity()
	{
	}
	void EmptyEntity::Start()
	{
	}
	void EmptyEntity::Update(float deltaTime)
	{
	}
	void EmptyEntity::Render()
	{
	}
	void EmptyEntity::Serialize(nlohmann::json& json)
	{
	}
	void EmptyEntity::Deserialize(nlohmann::json& json)
	{
	}
	void EmptyEntity::DrawEditorUI()
	{
	}
}