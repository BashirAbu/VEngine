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
			flecs::entity entity;
			int32_t renderOrder;
		};

		struct Model3D
		{
			Model model;
			flecs::entity entity;
		};

		void Submit(Tex2D& texture2D);
		void SubmitUI(Tex2D& UITexture);

		void Submit(Model3D& model);
		
		void BeginFrame();
		RenderTexture GetMainRenderTarget() { return mainRenderTarget; }
	private:
		RenderTexture mainRenderTarget;
		std::vector<Tex2D> texture2DRenderQueue;
		std::mutex texture2DRenderQueueMutex;

		std::vector<Tex2D> UIRenderQueue;
		std::mutex UIRenderQueueMutex;

		std::vector<Model3D> model3DRenderQueue;
		std::mutex model3DRenderQueueMutex;
		class Scene* scene;
		friend class Editor;
	};
}