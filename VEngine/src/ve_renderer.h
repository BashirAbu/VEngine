#pragma once
#include "ve_defines.h"
#include <raylib.h>
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
		void RenderQueued2D();
		void RenderQueued3D();
		void RenderUIQueued();
		void RenderScene();
		struct Tex2D
		{
			Texture2D texture;
			Rectangle source;
			Rectangle dest;
			Vector2 origin;
			float rotation;
			Color tint;
		};
		void Submit(Tex2D& texture2D, int32_t renderOrder, flecs::entity e);
		void SubmitUI(Tex2D& UITexture, int32_t renderOrder, flecs::entity e);
		
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

		std::vector<FullTex2D> UIRenderQueue;
		std::mutex UIRenderQueueMutex;
		class Scene* scene;
		friend class Editor;
	};
}