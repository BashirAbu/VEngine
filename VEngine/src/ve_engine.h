#pragma once
#include "ve_defines.h"
#include "scenes/ve_scene_manager.h"
#include "platform/ve_shared_library.h"
#ifdef VE_EDITOR
#include "editor/ve_editor.h"
#endif

#ifndef VE_EDITOR
extern "C" void OnSharedLibraryEntry(flecs::world& world);
extern "C" void ProjectDrawComponentElements(std::string name, flecs::entity entity);
#endif
namespace VE 
{
	extern std::vector<std::string>* logs;
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
	};

#ifdef VE_EDITOR
	typedef VE_API void (*PFN_OnSharedLibraryEntry)(flecs::world& world);
	inline VE_API PFN_OnSharedLibraryEntry OnSharedLibraryEntry;
	typedef VE_API void (*PFN_ProjectDrawComponentElements)(std::string name, flecs::entity entity);
	inline VE_API PFN_ProjectDrawComponentElements ProjectDrawComponentElements;
#endif
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
#ifdef VE_EDITOR
		Editor* GetEditor() { return editor; }
#endif
	private:
#ifdef VE_EDITOR
		Editor* editor;
#endif

		EngineDesc desc;
		SceneManager* sceneManager;
#ifdef VE_EDITOR
		VE::SharedLibrary* projectSharedLibrary;
#endif
		static Engine* singleton;
		friend class Editor;
	};
}



