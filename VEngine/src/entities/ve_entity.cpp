#include "ve_engine.h"
#include "ve_entity.h"
#include <imgui.h>
#include "ve_engine.h"
namespace VE 
{
	Entity::Entity(std::string name) : parent(nullptr)
	{
		this->name = name;
		internalName = name;
		transformComponent = new TransformComponent(this);
		components.push_back(transformComponent);

		VE::Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->entities.push_back(this);
	}
	Entity::~Entity()
	{
		for (auto itr = children.begin(); itr != children.end();)
		{
			Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->entities.remove(*itr);
			delete (*itr);
		}
		children.clear();
		for (Component* comp : components)
		{
			delete comp;
		}
		components.clear();
	}
	void Entity::SetParent(Entity* parent)
	{
		this->parent = parent;
	}
	void Entity::AddChild(Entity* child)
	{
		child->parent = this;
		children.push_back(child);
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