#include "ve_engine.h"
#include "ve_entity.h"
#include <imgui.h>
#include "scenes/ve_scene_manager.h"
#include "ve_engine.h"
namespace VE 
{
	static int32_t idGenerator = 0;
	Entity::Entity(std::string name) : parent(nullptr)
	{
		id = idGenerator++;
		this->name = name;
		internalName = name;
		transformComponent = new TransformComponent(this);
		components.push_back(transformComponent);

		VE::Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->entities.push_back(this);
		VE::Engine::GetSingleton()->GetSceneManager()->GetCurrentScene()->entityIDtable[id] = this;
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
	Scene* Entity::GetCurrentScene()
	{
		return VE::Engine::GetSingleton()->GetSceneManager()->GetCurrentScene();
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
	void Entity::ComponentsPreUpdate(float deltaTime)
	{
		for (Component* comp : components)
		{
			comp->PreUpdate(deltaTime);
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
		char buffer[255];
		if (name.size() > 255)
		{
			name.resize(255);
		}
		strcpy(buffer, name.c_str());
		ImGui::InputText("Name", buffer, 255);
		name = buffer;

		for (Component* comp : components)
		{
			comp->DrawEditorUI();
			ImGui::Separator();
		}
	}
}