#pragma once
#include <filesystem>
#include "ve_defines.h"
#include <string>
#include "scenes/ve_scene_manager.h"
#include "platform/ve_shared_library.h"
#include <memory>
#include "editor/ve_editor.h"
namespace VE 
{
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

	typedef VE_API void (*PFN_OnSharedLibraryEntry)();
	inline VE_API PFN_OnSharedLibraryEntry OnSharedLibraryEntry;
	typedef VE_API Entity* (*PFN_CreateProjectEntity) (std::string);
	inline VE_API PFN_CreateProjectEntity CreateProjectEntity;
	class VE_API Engine
	{
	public:
		Engine(EngineDesc engineDesc);
		~Engine();

		static Engine* GetSingleton();

		void Run();
		//use this for both builtin entities and project entities.
		void RegisterEntity(std::string entityName);
		// use this for adding builtin entity such as camera entity.
		class Entity* CreateBuiltinEntity(std::string entityName);

		void LoadProjectSharedLibrary();
		void ReloadProjectSharedLibrary();
		void UnloadProjectSharedLibrary();



		const SceneManager* GetSceneManager() const { return sceneManager; };
		const EngineDesc* GetDesc() const { return &desc; }



	private:
		Editor* editor;

		EngineDesc desc;
		SceneManager* sceneManager;
		VE::SharedLibrary* projectSharedLibrary;
		std::list<std::string> entitiesRegistry;
		static Engine* singleton;
		friend class Editor;
	};
}



