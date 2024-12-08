#pragma once
#include "ve_defines.h"
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
		SceneMode GetMode() { return mode; }

		void ReloadCurrentScene();
		void ChangeScene(std::filesystem::path filepath);
	private:
		bool reloadCurrentScene = false;
		bool changeScene = false;
		std::filesystem::path changeScenePath = "";
		Scene* currentScene;
		SceneMode mode;
		friend class Editor;
		friend class Engine;
		friend class Scene;
	};
}