#pragma once
#include "ve_defines.h"
#include "scenes/ve_scene_manager.h"
#include "platform/ve_shared_library.h"
#include "editor/ve_editor.h"
namespace VE 
{
	extern std::vector<std::string>* logs;
	enum class VE_API RuntimeType
	{
		Editor,
		Game
	};
	struct VE_API ProjectDetails
	{
		std::string name;
		std::string title;
		std::filesystem::path path;
		std::filesystem::path mainScenePath;
		std::filesystem::path assetsPath;
		uint32_t width;
		uint32_t height;
		uint32_t renderWidth;
		uint32_t renderHeight;
		bool vsync;
	};
	struct VE_API EngineDesc
	{
		ProjectDetails projectDetails;
		RuntimeType runtimeType;
	};

	typedef VE_API void (*PFN_OnSharedLibraryEntry)(flecs::world& world);
	inline VE_API PFN_OnSharedLibraryEntry OnSharedLibraryEntry;

	class VE_API Engine
	{
	public:
		Engine(EngineDesc engineDesc);
		~Engine();

		static Engine* GetSingleton();

		void Run();

		void LoadProjectSharedLibrary();
		void ReloadProjectSharedLibrary();
		void UnloadProjectSharedLibrary();



		SceneManager* GetSceneManager() { return sceneManager; };
		const EngineDesc* GetDesc() const { return &desc; }

		Editor* GetEditor() { return editor; }

	private:
		Editor* editor;

		EngineDesc desc;
		SceneManager* sceneManager;
		VE::SharedLibrary* projectSharedLibrary;

		static Engine* singleton;
		friend class Editor;
	};
}



