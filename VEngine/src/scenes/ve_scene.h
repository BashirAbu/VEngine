#pragma once
#include "ve_defines.h"
#include <glm/glm.hpp>
#include <list>
#include <raylib.h>
#include <filesystem>
namespace VE 
{
	class Entity;
	enum class SceneType 
	{
		Scene2D,
		Scene3D
	};
	class VE_API Scene
	{
	public:
		Scene();
		~Scene();

		void Start();
		void Update(float deltaTime);
		virtual void Render() = 0;
		void DrawEditorUI();

		void AddEntity(Entity* entity);
		void RemoveEntity(Entity* entity);
		virtual void SetMainCamera(class Entity* camera) = 0;

		const SceneType GetSceneType() const { return sceneType; }
		const RenderTexture GetEditorCameraRenderTarget() const { return editorCameraRenderTarget; }
		virtual const Entity* GetMainCamera() const = 0;
	protected:
		void DeleteEntityChildren(Entity* entity);
		std::string name;
		bool started = false;
		std::filesystem::path scenePath;
		std::list<class Entity*> cameras;
		std::list<class Entity*> entities;
		RenderTexture editorCameraRenderTarget;
		SceneType sceneType;

		friend class Editor;
		friend class Entity;
		friend class Camera2DEntity;
		friend class SceneManager;
	};
}