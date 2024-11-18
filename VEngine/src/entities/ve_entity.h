#pragma once
#include "ve_defines.h"
#include <string>
#include "components/ve_transform_component.h"
#include <list>
#include <memory>
#include <bitset>
namespace VE 
{
	typedef std::bitset<128> LayerMask;
	class VE_API Entity
	{
	public:
		Entity(std::string name = "");

		virtual ~Entity();

		virtual void Start() = 0;
		virtual void PreUpdate(float deltaTime) {};
		virtual void Update(float deltaTime) = 0;
		virtual void Render() {}
		virtual void Serialize(nlohmann::json& json) {}
		virtual void Deserialize(nlohmann::json& json) {}
		virtual void DrawEditorUI() {}

		class Scene* GetCurrentScene();

		template <typename T>
		T* AddComponent();
		template <typename T>
		T* GetComponent();
		template <typename T>
		void RemoveComponent();

		void SetParent(Entity* parent);
		void AddChild(Entity* child);

		Entity* GetParent() { return parent; }
		std::list<Entity*> GetChildren() { return children; }
		std::string GetTag() { return tag; }
		void Destroy() { this->destroy = true; }
		void SetInternalName(std::string internalName) { this->internalName = internalName; };

		TransformComponent* transformComponent;
		std::string name;
		std::string tag;
	protected:
		LayerMask layer;
	private:
		void ComponentsStart();
		void ComponentsUpdate(float deltaTime);
		void ComponentsPreUpdate(float deltaTime);
		void ComponentsRender();
		void ComponentsSerialize(nlohmann::json& json);
		void ComponentsDeserialize(nlohmann::json& json);
		void ComponentDrawEditorUI();

		std::string internalName;
		std::list<Component*> components;

		Entity* parent;
		std::list<Entity*> children;
		bool started = false;
		bool construct = false;
		bool destroy = false;
		std::filesystem::path constructPath;

		int32_t id = -1;
		friend class Scene;
		friend class SceneManager;
		friend class Engine;
		friend class Editor;
		friend class Scene2D;
		friend class Scene3D;
	};

	template <typename T>
	T* Entity::AddComponent()
	{
		T* comp = new T(this);
		components.push_back(comp);
		return comp;
	}
	template <typename T>
	T* Entity::GetComponent()
	{
		for (Component* comp : components)
		{
			if (dynamic_cast<T*>(comp))
			{
				return (T*)comp;
			}
		}
		return nullptr;
	}
	template <typename T>
	void Entity::RemoveComponent()
	{
		for (auto itr = components.begin(); itr != components.end(); itr++)
		{
			if (dynamic_cast<T*>(*itr))
			{
				components.erase(itr);
				return;
			}
		}
	}
}