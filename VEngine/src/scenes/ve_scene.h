#pragma once
#include "ve_defines.h"
#include <glm/glm.hpp>
#include <raylib.h>
#include <filesystem>
#include <flecs.h>
#include <queue>
#include <mutex>
#include <concurrent_queue.h>
namespace VE 
{
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

		static Scene* GetSingleton() { return singleton; }

		void Start();
		void Update();
		void Render();

		void SetMainCamera(flecs::entity entity);

		flecs::entity AddEntity(std::string name);
		std::string AddConstruct(std::filesystem::path constructFilePath);
		const SceneType GetSceneType() const { return sceneType; }
		void AddMetaData(flecs::entity comp);

		std::unordered_map<std::string, flecs::entity>* GetSceneSystems() 
		{
			return &sceneSystems;
		}

		flecs::entity CloneEntity(flecs::entity entity);

		flecs::entity LookupEntity(std::string name);

		bool LookupEntityName(std::string name);


		struct Texture2DRenderQueue
		{
			Texture2D* texture;
			Rectangle source;
			Rectangle dest;
			Vector2 origin;
			float rotation;
			Color tint;
		};

		Concurrency::concurrent_queue<Texture2DRenderQueue> texture2DRenderQueue;



	private:
		void CloneChildren(flecs::entity entity, flecs::entity cloneParent);
		std::string GenUniqueName(std::string name);
		flecs::world world;
		glm::vec4 clearColor;


		std::unordered_map<std::string, flecs::entity> sceneSystems;

		std::unordered_map<std::string, flecs::entity> componentsTable;
		std::unordered_map<std::string, flecs::entity> systemsTable;
		std::unordered_map<std::string, flecs::entity> cachedEntitiesTable;
		
		SceneType sceneType;
		bool started = false;

		std::filesystem::path scenePath = "";

		std::vector<std::filesystem::path> deferredConstructs;


		friend class Editor;
		friend class SceneManager;

		static Scene* singleton;
	};
}