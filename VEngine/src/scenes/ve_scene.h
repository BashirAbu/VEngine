#pragma once
#include "ve_defines.h"
#include <glm/glm.hpp>
#include <raylib.h>
#include <filesystem>
#include <flecs.h>
#include <queue>
#include <mutex>
#include <concurrent_queue.h>
#include "ve_renderer.h"
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
		Scene(SceneType type, std::string flecsJson = "");
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

		flecs::entity CloneEntity(flecs::entity entity);
		flecs::entity LookupEntity(std::string name);
		//You can use int for id.
		flecs::entity LookupEntity(flecs::entity id);

		//System render phase
		flecs::entity OnRender;
		flecs::entity updatePipeline;
		flecs::entity renderPipeline;
		Renderer renderer;

		std::string SerializeScene();
		std::string SerializeEntity(flecs::entity e);

	private:
		void CloneChildren(flecs::entity entity, flecs::entity cloneParent);
		std::string GenUniqueName(std::string name);
		flecs::world world;
		glm::vec4 clearColor;

		std::unordered_map<std::string, flecs::entity> componentsTable;

		struct SystemInfo 
		{
			flecs::entity entity;
			bool enable = false;
		};

		std::unordered_map<std::string, SystemInfo> systemsTable;
		
		SceneType sceneType;
		bool started = false;

		std::filesystem::path scenePath = "";

		std::vector<std::filesystem::path> deferredConstructs;

		friend class Editor;
		friend class SceneManager;
		friend class Renderer;
		static Scene* singleton;
	};
}