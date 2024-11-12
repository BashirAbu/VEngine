#include "ve_engine.h"
#include "ve_entity.h"
#include <imgui.h>
namespace VE 
{
	Entity::Entity(std::string name)
	{
		this->name = name;
		transformComponent = new TransformComponent(this);
		components.push_back(transformComponent);
	}
	Entity::~Entity()
	{
		for (Component* comp : components)
		{
			delete comp;
		}
		components.clear();
	}
	void Entity::ComponentsStart()
	{
		for (Component* comp : components)
		{
			comp->Start();
		}
	}
	void Entity::ComponentsUpdate(float deltaTime)
	{
		for (Component* comp : components)
		{
			comp->Update(deltaTime);
		}
	}
	void Entity::ComponentsRender()
	{
		for (Component* comp : components)
		{
			comp->Render();
		}
	}
	void Entity::ComponentsSerialize(nlohmann::json& json)
	{
		for (Component* comp : components)
		{
			comp->Serialize(json);
		}
	}
	void Entity::ComponentsDeserialize(nlohmann::json& json)
	{
		for (Component* comp : components) 
		{
			comp->Deserialize(json);
		}
	}
	void Entity::ComponentDrawEditorUI()
	{
		for (Component* comp : components)
		{
			comp->DrawEditorUI();
			ImGui::Separator();
		}
	}
}