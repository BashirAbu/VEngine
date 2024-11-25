#pragma once
#include "ve_defines.h"
#include <glm/glm.hpp>
#include <raylib.h>
#include <filesystem>
#include <flecs.h>
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

		void Start();
		void Update();
		void Render();

		void SetMainCamera(flecs::entity entity);

		flecs::entity AddEntity(std::string name);
		flecs::entity AddConstruct(std::filesystem::path constructFilePath);
		const SceneType GetSceneType() const { return sceneType; }
		void AddMetaData(flecs::entity comp);

		std::unordered_map<std::string, flecs::entity>* GetSceneSystems() 
		{
			return &sceneSystems;
		}

	private:
		flecs::world world;
		glm::vec4 clearColor;


		std::unordered_map<std::string, flecs::entity> sceneSystems;

		std::unordered_map<std::string, flecs::entity> componentsTable;
		std::unordered_map<std::string, flecs::entity> systemsTable;
		
		SceneType sceneType;
		bool started = false;
		size_t entityIndexGen = 0;

		std::filesystem::path scenePath = "";

		friend class Editor;
		friend class SceneManager;
	};
}