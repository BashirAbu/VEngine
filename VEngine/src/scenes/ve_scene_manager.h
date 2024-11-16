#pragma once
#include <filesystem>
#include "ve_defines.h"
#include <string>
#include <memory>
#include "ve_scene.h"
#include <nlohmann/json.hpp>
namespace VE 
{
	enum class VE_API SceneMode 
	{
		Editor,
		Game
	};
	class VE_API SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void LoadScene(std::filesystem::path scenePath);
		void UnloadScene();
		void RunCurrentScene();
		//editor function
		void SaveSceneAs();
		//editor function
		void SaveScene();
		nlohmann::json SerializeChildren(Entity* entity, size_t& index);
		Entity* DeserializeChildren(nlohmann::json entityJson);
		Scene* CreateEmptyScene(SceneType type);

		Scene* GetCurrentScene() { return currentScene; }

		Entity* LoadConstruct(std::filesystem::path constructPath);

		SceneMode GetSceneMode() { return mode; }
	private:
		Scene* currentScene;
		SceneMode mode;
		Entity* selectedEntity;
		friend class Editor;
		friend class Engine;
		friend class Scene;
		friend class Scene2D;
		friend class Camera2DEntity;
	};
}