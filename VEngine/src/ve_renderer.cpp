#include "ve_renderer.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include <scenes/ve_scene.h>
#include <rlgl.h>

namespace VE 
{
	Renderer::Renderer(Scene* scene) : scene(scene)
	{
		mainRenderTarget = LoadRenderTexture(VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderWidth,
			VE::Engine::GetSingleton()->GetDesc()->projectDetails.renderHeight,
			PIXELFORMAT_UNCOMPRESSED_R8G8B8);

		SetTextureFilter(mainRenderTarget.texture, TEXTURE_FILTER_BILINEAR);
		SetTextureWrap(mainRenderTarget.texture, TEXTURE_WRAP_CLAMP);
	}
	Renderer::~Renderer()
	{
		UnloadRenderTexture(mainRenderTarget);
	}

	void Renderer::RenderQueued2D()
	{
		for (const auto& t2d : texture2DRenderQueue)
		{
			if (t2d.shader.id > 0)
			{
				BeginShaderMode(t2d.shader);
			}
			DrawTexturePro(t2d.texture, t2d.source, t2d.dest, t2d.origin, t2d.rotation, t2d.tint);
			if (t2d.shader.id > 0)
			{
				EndShaderMode();
			}
		}
	}

	void Renderer::RenderQueued3D()
	{
		for (const auto& m3d : model3DRenderQueue)
		{
			DrawModel(m3d.model, {}, 1.0f, WHITE);
		}
	}

	void Renderer::RenderUIQueued()
	{
		for (const auto& t2d : UIRenderQueue)
		{
			DrawTexturePro(t2d.texture, t2d.source, t2d.dest, t2d.origin, t2d.rotation, t2d.tint);
		}
	}

	void Renderer::RenderScene()
	{
		//Editor stuff

#ifdef VE_EDITOR
		if (Engine::GetSingleton()->GetSceneManager()->GetMode() == SceneMode::Editor)
		{
			BeginFrame();

			flecs::entity ts = scene->systemsTable["Camera2DSystem"].entity;
			flecs::system* tss = (flecs::system*)&ts;
			tss->run();

			ts = scene->systemsTable["TransformSystem"].entity;
			tss = (flecs::system*)&ts;
			tss->run();

			ts = scene->systemsTable["UICanvasSystem"].entity;
			tss = (flecs::system*)&ts;
			tss->run();

			ts = scene->systemsTable["Camera3DSystem"].entity;
			tss = (flecs::system*)&ts;
			tss->run();
		}
#endif
		//render
		scene->world.set_pipeline(scene->renderPipeline);
		scene->world.progress();

		flecs::query cameras2D = scene->world.query<Components::Camera2DComponent>();

		std::sort(texture2DRenderQueue.begin(), texture2DRenderQueue.end(), [](Tex2D& a, Tex2D& b) 
			{
				return b.renderOrder > a.renderOrder;
			});

		cameras2D.each([&](flecs::entity e, Components::Camera2DComponent& cc)
			{
				if (!e.has<_Components::Disabled>())
				{
					BeginTextureMode(cc.renderTarget);
					BeginMode2D(cc.camera);
					ClearBackground(GLMVec4ToRayColor(cc.backgroundColor));

					RenderQueued2D();

					EndMode2D();
					EndTextureMode();
				}

			});

		flecs::query cameras3D = scene->world.query<Components::Camera3DComponent>();

		cameras3D.each([&](flecs::entity e, Components::Camera3DComponent& cc)
			{
				if (!e.has<_Components::Disabled>())
				{
					BeginTextureMode(cc.renderTarget);
					BeginMode3D(cc.camera);
					ClearBackground(GLMVec4ToRayColor(cc.backgroundColor));
					if (cc.skyboxTexture)
					{
						rlDisableBackfaceCulling();
						rlDisableDepthMask();
						DrawModel(cc.skyboxModel, { 0, 0, 0 }, 1.0f, WHITE);
						rlEnableBackfaceCulling();
						rlEnableDepthMask();
					}
					RenderQueued3D();

					EndMode3D();
					EndTextureMode();
				}

			});


		flecs::query uiCanvas = scene->world.query<Components::UI::UICanvasComponent>();


		std::sort(UIRenderQueue.begin(), UIRenderQueue.end(), [](Tex2D& a, Tex2D& b)
			{
				return b.renderOrder > a.renderOrder;
			});

		uiCanvas.each([&](flecs::entity e, Components::UI::UICanvasComponent& canvas) 
			{
				if (!e.has<_Components::Disabled>())
				{
					BeginTextureMode(canvas.canvasRenderTarget);

					ClearBackground(BLANK);

					RenderUIQueued();

					EndTextureMode();
				}

			});

		//Draw UI render Target

		BeginTextureMode(mainRenderTarget);
		ClearBackground(BLANK);

		

		flecs::entity mainCameraEntity = scene->GetMainCamera();
		if (mainCameraEntity)
		{
			Components::Camera2DComponent* mainCamera2D = mainCameraEntity.get_mut<Components::Camera2DComponent>();
			Components::Camera3DComponent* mainCamera3D = mainCameraEntity.get_mut<Components::Camera3DComponent>();
			if (mainCamera2D)
			{
				RaylibDrawTexturTargeteLetterBox(mainCamera2D->renderTarget, { (float)mainRenderTarget.texture.width, (float)mainRenderTarget.texture.height });
			}
			else if (mainCamera3D)
			{
				RaylibDrawTexturTargeteLetterBox(mainCamera3D->renderTarget, { (float)mainRenderTarget.texture.width, (float)mainRenderTarget.texture.height });
			}
		}

		flecs::entity mainCanvasEntity = scene->world.query<Components::UI::UICanvasComponent>().find([](flecs::entity e, Components::UI::UICanvasComponent& canvas)
			{
				return canvas.isMain && !e.has<_Components::Disabled>() ? e : flecs::entity();
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
	void Renderer::Submit(Tex2D& texture2D)
	{
		std::lock_guard<std::mutex> lock(texture2DRenderQueueMutex);
		texture2DRenderQueue.push_back(texture2D);
	}
	void Renderer::SubmitUI(Tex2D& UITexture)
	{
		std::lock_guard<std::mutex> lock(UIRenderQueueMutex);
		UIRenderQueue.push_back(UITexture);
	}

	void Renderer::Submit(Model3D& model)
	{
		std::lock_guard<std::mutex> lock(model3DRenderQueueMutex);
		model3DRenderQueue.push_back(model);
	}

	void Renderer::BeginFrame()
	{
		texture2DRenderQueue.clear();
		UIRenderQueue.clear();
		model3DRenderQueue.clear();
	}
}