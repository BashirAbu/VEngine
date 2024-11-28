#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include <concurrent_vector.h>

namespace VE 
{
	class VE_API Renderer 
	{
	public:
		Renderer(class Scene* scene);
		~Renderer();
		void RenderQueued();
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
		void BeginFrame();
	private:
		struct FullTex2D 
		{
			Tex2D texture;
			flecs::entity entity;
			int32_t renderOrder;
		};
		Concurrency::concurrent_vector<FullTex2D> texture2DRenderQueue;
		class Scene* scene;
		friend class Editor;
	};
}