#pragma once

#include "ve_defines.h"
#include <glm/glm.hpp>
#include <raylib.h>
#include <string>
#include <vector>
#include "imgui.h"
struct ImFont;
namespace VE 
{
	void AddLog(const std::string& message);
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
		void RenderEditorSceneView();
		void AddEntityNode(class Entity* entity, std::list<Entity*>& deletedEntities);
		void DrawChildren(class Entity* entity, std::list<Entity*>& deletedEntities);
		const bool IsGameViewportFocused() const { return gameViewportFocused; }
		const bool IsSceneViewportFocused() const { return sceneViewportFocused; }
		const glm::vec2 GetSceneViewportSize() const { return sceneViewportSize; }
		const glm::vec2 GetGameViewportSize() const { return gameViewportSize; }
		const glm::vec2 GetSceneViewportPosition() const { return sceneViewportPosition; }
		const glm::vec2 GetGameViewportPosition() const { return gameViewportPosition; }
	private:
		glm::vec2 sceneViewportSize;
		glm::vec2 gameViewportSize;
		glm::vec2 sceneViewportPosition;
		glm::vec2 gameViewportPosition;
		bool gameViewportFocused = false;
		bool sceneViewportFocused = false;
		bool isSceneViewHovered = false;
		bool usingImGuizmo = false;
		class Engine* engine;

		Camera2D editorCamera;
		glm::vec2 oldRenderTargetSize;
		RenderTexture editorCameraRenderTarget;

		RenderTexture colorPickingBuffer;
		Shader colorPickingShader;

		ImFont* font;
		friend class Engine;

		static Editor* singleton;
	};
}