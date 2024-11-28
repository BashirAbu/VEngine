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
	private:

		Scene* currentScene;
		SceneMode mode;
		friend class Editor;
		friend class Engine;
		friend class Scene;
	};
}