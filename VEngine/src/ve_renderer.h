#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include <concurrent_vector.h>
#include <mutex>
#include <thread>
#include "ve_font.h"

namespace VE 
{
	class VE_API Renderer 
	{
	public:
		Renderer(class Scene* scene);
		~Renderer();
		void RenderQueued();
		void RenderUIQueued();
		void RenderScene();
		struct Tex2D
		{
			Texture2D* texture;
			Rectangle source;
			Rectangle dest;
			Vector2 origin;
			float rotation;
			Color tint;
		};
		void Submit(Tex2D& texture2D, int32_t renderOrder, flecs::entity e);
		struct Label2D 
		{
			VE::Font* font;
			std::string text;
			glm::vec3 position;
			glm::mat4 worldTransformMatrix;
			glm::vec2 origin;
			float rotation;
			float fontSize;
			float spacing;
			Color tint;

			std::wstring wideString = L"";
		};
		void Submit(Label2D& label, int32_t renderOrder, flecs::entity e);
		void BeginFrame();
		RenderTexture GetMainRenderTarget() { return mainRenderTarget; }
	private:
		RenderTexture mainRenderTarget;
		struct FullTex2D 
		{
			Tex2D texture;
			flecs::entity entity;
			int32_t renderOrder;
		};
		std::vector<FullTex2D> texture2DRenderQueue;
		std::mutex texture2DRenderQueueMutex;

		struct FullLabel2D 
		{
			Label2D label;
			flecs::entity entity;
			int32_t renderOrder;
		};

		

		std::vector<FullLabel2D> label2DRenderQueue;
		std::mutex label2DRenderQueueMutex;
		class Scene* scene;
		friend class Editor;
	};
}