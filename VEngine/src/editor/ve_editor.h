#pragma once
#ifdef VE_EDITOR
#include "ve_defines.h"
#include <raylib.h>
#include "imgui.h"
#include <queue>
struct ImFont;
namespace VE 
{
	void AddLog(const std::string message);
	class VE_API Editor 
	{
	public:
		Editor(class Engine* engine);
		~Editor();
		static Editor* GetSingleton();
		void DrawUI();
		void DrawHierarchy();
		void DrawMainMenuBar();
		void DrawInspector();
		void DrawStatus();
		void DrawSceneViewport();
		void DrawGameViewport();


		glm::vec2 SceneViewportMousePos();

		class ConsoleWindow 
		{
		public:
			ConsoleWindow();
			~ConsoleWindow();

			void Draw();
			void ClearLog();
		};

		ConsoleWindow consoleWindow;

		void UpdateEditor(float deltaTime);
		void AddEntityNode(flecs::entity entity);
		void AddChildrenNode(flecs::entity e);
		const bool IsGameViewportFocused() const { return gameViewportFocused; }
		const bool IsSceneViewportFocused() const { return sceneViewportFocused; }
		const glm::vec2 GetSceneViewportSize() const { return sceneViewportSize; }
		const glm::vec2 GetGameViewportSize() const { return gameViewportSize; }
		const glm::vec2 GetSceneViewportPosition() const { return sceneViewportPosition; }
		const glm::vec2 GetGameViewportPosition() const { return gameViewportPosition; }

		void DrawComponentElements(std::string name, flecs::entity entity);

	private:
		std::string tempSelecetedEntity;
		std::filesystem::path reloadScenePath;

		std::string entityName;
		bool saveName = false;
		flecs::entity selectedEntity;

		glm::vec2 sceneViewportSize = {};
		glm::vec2 gameViewportSize = {};
		glm::vec2 sceneViewportPosition = {};
		glm::vec2 gameViewportPosition = {};
		bool gameViewportFocused = false;
		bool sceneViewportFocused = false;
		bool isSceneViewHovered = false;
		bool usingImGuizmo = false;
		class Engine* engine;

		std::queue<std::string> addEntities;
		std::queue<flecs::entity> cloneEntities;
		std::queue<flecs::entity> removeEntities;
		std::queue<flecs::entity> addChildQueue;

		enum class CameraMode 
		{
			CAMERA2D = 0,
			CAMERA3D
		};
		CameraMode cameraMode = CameraMode::CAMERA2D;
		::Camera2D editorCamera2D;
		::Camera editorCamera3D;
		float editorCameraMoveSpeed = 10.0f;
		float editorCameraSensitiviy = 0.003f;
		float editorCameraZoom = 20.0f;


		glm::vec2 oldRenderTargetSize;
		RenderTexture editorCameraRenderTarget;

		RenderTexture colorPickingBuffer;
		Shader colorPickingShader;

		ImFont* font;
		ImFont* consoleFont;
		friend class Engine;

		static Editor* singleton;
	};
}
#endif