#include "ve_renderer.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include <scenes/ve_scene.h>
namespace VE 
{
	Renderer::Renderer(Scene* scene) : scene(scene)
	{
	}
	Renderer::~Renderer()
	{
	}

	void Renderer::RenderQueued()
	{
		for (const auto& t2d : texture2DRenderQueue)
		{
			DrawTexturePro(*t2d.texture.texture, t2d.texture.source, t2d.texture.dest, t2d.texture.origin, t2d.texture.rotation, t2d.texture.tint);
		}
	}

	void Renderer::RenderScene()
	{
		//Editor stuff
		if (Engine::GetSingleton()->GetSceneManager()->GetMode() == SceneMode::Editor)
		{
			BeginFrame();

			flecs::entity ts = scene->systemsTable["Camera2DTransformSystem"].entity;
			flecs::system* tss = (flecs::system*)&ts;

			tss->run();
		}

		//render
		scene->world.set_pipeline(scene->renderPipeline);

		scene->world.progress();

		flecs::query cameras2D = scene->world.query<Components::Camera2DComponent>();

		std::sort(texture2DRenderQueue.begin(), texture2DRenderQueue.end(), [](FullTex2D& a, FullTex2D& b) 
			{
				return b.renderOrder > a.renderOrder;
			});

		cameras2D.each([&](flecs::entity e, Components::Camera2DComponent& cc)
			{
				BeginTextureMode(cc.renderTarget);
				BeginMode2D(cc.camera);
				ClearBackground(GLMVec4ToRayColor(cc.backgroundColor));

				RenderQueued();

				EndMode2D();
				EndTextureMode();
			});
	}
	void Renderer::Submit(Tex2D& texture2D, int32_t renderOrder, flecs::entity e)
	{
		FullTex2D t2d;
		t2d.entity = e;
		t2d.renderOrder = renderOrder;
		t2d.texture = texture2D;
		texture2DRenderQueue.push_back(t2d);
	}
	void Renderer::BeginFrame()
	{
		texture2DRenderQueue.clear();
	}
}