#include "ve_renderer.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include <scenes/ve_scene.h>
namespace VE 
{
	Renderer::Renderer(Scene* scene) : scene(scene)
	{
		mainRenderTarget = LoadRenderTexture(VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
			VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight,
			PIXELFORMAT_UNCOMPRESSED_R8G8B8);

		SetTextureFilter(mainRenderTarget.texture, TEXTURE_FILTER_BILINEAR);
	}
	Renderer::~Renderer()
	{
		UnloadRenderTexture(mainRenderTarget);
	}

	void Renderer::RenderQueued()
	{
		for (const auto& t2d : texture2DRenderQueue)
		{
			DrawTexturePro(*t2d.texture.texture, t2d.texture.source, t2d.texture.dest, t2d.texture.origin, t2d.texture.rotation, t2d.texture.tint);
		}
	}

	void Renderer::RenderUIQueued()
	{
		for (const auto& l2d : label2DRenderQueue)
		{
			//DrawTextPro(*l2d.label.font, l2d.label.text, l2d.label.position, l2d.label.origin, l2d.label.rotation, l2d.label.fontSize, l2d.label.spacing, l2d.label.tint);
			RaylibDrawTextUTF8(l2d.label);
		}
	}

	void Renderer::RenderScene()
	{
		//Editor stuff
		if (Engine::GetSingleton()->GetSceneManager()->GetMode() == SceneMode::Editor)
		{
			BeginFrame();

			flecs::entity ts = scene->systemsTable["Camera2DSystem"].entity;
			flecs::system* tss = (flecs::system*)&ts;
			tss->run();

			ts = scene->systemsTable["TransformSystem"].entity;
			tss = (flecs::system*)&ts;
			tss->run();

			ts = scene->systemsTable["CanvasSystem"].entity;
			tss = (flecs::system*)&ts;
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

		flecs::query uiCanvas = scene->world.query<Components::UI::UICanvasComponent>();


		std::sort(label2DRenderQueue.begin(), label2DRenderQueue.end(), [](FullLabel2D& a, FullLabel2D& b)
			{
				return b.renderOrder > a.renderOrder;
			});

		uiCanvas.each([&](flecs::entity e, Components::UI::UICanvasComponent& canvas) 
			{
				BeginTextureMode(canvas.canvasRenderTarget);

				ClearBackground(BLANK);

				RenderUIQueued();

				EndTextureMode();
			});

		//Draw UI render Target

		BeginTextureMode(mainRenderTarget);
		ClearBackground(BLANK);

		flecs::entity mainCameraEntity = scene->world.query<Components::Camera2DComponent>().find([](flecs::entity e, Components::Camera2DComponent& camera)
			{
				return camera.isMain ? e : flecs::entity();
			});

		if (mainCameraEntity)
		{
			Components::Camera2DComponent* mainCamera = mainCameraEntity.get_mut<Components::Camera2DComponent>();
			if (mainCamera)
			{
				RaylibDrawTexturTargeteLetterBox(mainCamera->renderTarget, { (float)mainRenderTarget.texture.width, (float)mainRenderTarget.texture.height });

			}
		}

		flecs::entity mainCanvasEntity = scene->world.query<Components::UI::UICanvasComponent>().find([](flecs::entity e, Components::UI::UICanvasComponent& canvas)
			{
				return canvas.isMain ? e : flecs::entity();
			});

		if (mainCanvasEntity)
		{
			Components::UI::UICanvasComponent* mainCanvas = mainCanvasEntity.get_mut<Components::UI::UICanvasComponent>();
			if (mainCanvas)
			{
				RaylibDrawTexturTargeteLetterBox(mainCanvas->canvasRenderTarget, { (float)mainRenderTarget.texture.width, (float)mainRenderTarget.texture.height });
			}
		}
		EndTextureMode();

	}
	void Renderer::Submit(Tex2D& texture2D, int32_t renderOrder, flecs::entity e)
	{
		FullTex2D t2d;
		t2d.entity = e;
		t2d.renderOrder = renderOrder;
		t2d.texture = texture2D;
		std::lock_guard<std::mutex> lock(texture2DRenderQueueMutex);
		texture2DRenderQueue.push_back(t2d);
	}
	void Renderer::Submit(Label2D& label, int32_t renderOrder, flecs::entity e)
	{
		FullLabel2D l2d;
		l2d.entity = e;
		l2d.label = label;
		l2d.renderOrder = renderOrder;
		std::lock_guard<std::mutex> lock(label2DRenderQueueMutex);
		label2DRenderQueue.push_back(l2d);
	}
	void Renderer::BeginFrame()
	{
		texture2DRenderQueue.clear();
		label2DRenderQueue.clear();
	}
}