#pragma once
#include "ve_defines.h"
#include <glm/glm.hpp>
#include <list>
#include <raylib.h>
#include <filesystem>
#include <unordered_map>
#include <thread>
#include <mutex>
namespace VE 
{
	class Entity;
	enum class SceneType 
	{
		Scene2D,
		Scene3D
	};
	class VE_API Scene
	{
	public:
		Scene(SceneType type);
		~Scene();

		void Start();
		void Update();
		void Render();
		void DrawEditorUI();

		void AddEntity(Entity* entity);
		Entity* GetEntityByName(std::string name);
		void RemoveEntity(Entity* entity);
		void SetMainCamera(class CameraEntity* camera);

		const SceneType GetSceneType() const { return sceneType; }
		
		const class CameraEntity* GetMainCamera() const { return mainCamera; };
	protected:

		std::unordered_map<int32_t, Entity*> entityIDtable;

		glm::vec4 clearColor;

		void DeleteEntityChildren(Entity* entity);
		std::string name;
		bool started = false;
		std::filesystem::path scenePath;
		std::list<class CameraEntity*> cameras;
		std::list<class Entity*> entities;

		class CameraEntity* mainCamera = nullptr;

		SceneType sceneType;

		std::mutex entitiesMutex;
		std::thread updateThread;

		friend class Scene2D;
		friend class Editor;
		friend class Entity;
		friend class CameraEntity;
		friend class SceneManager;
	};
}