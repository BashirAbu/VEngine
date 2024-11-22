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

		void AddSystem(std::string name);
		flecs::entity AddEntity(std::string name);
		const SceneType GetSceneType() const { return sceneType; }

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