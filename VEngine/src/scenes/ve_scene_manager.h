#pragma once
#include <filesystem>
#include "ve_defines.h"
#include <string>
#include <memory>
#include "ve_scene.h"
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
		Scene* currentScene;
		SceneMode mode;
		Entity* selectedEntity;
	private:

	};
}