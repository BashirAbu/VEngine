#include "ve_scene.h"
#include "ve_engine.h"
#include "editor/ve_editor_input.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include "utils/ve_utils.h"
#include <imgui.h>
namespace VE 
{
	Scene::Scene(SceneType type)
	{
		sceneType = type;
		//register builtin components.
		world.component<std::string>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const std::string* data) 
			{
				const char* str = data->c_str();
				return s->value(flecs::String, &str);
			})
			.assign_string([](std::string* data, const char* value) 
			{
				*data = value;
			});
		world.component<std::filesystem::path>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const std::filesystem::path* data)
				{
					std::string temp = data->string();
					const char* str = temp.c_str();
					return s->value(flecs::String, &str);
				})
			.assign_string([](std::filesystem::path* data, const char* value)
				{
					*data = (std::string)value;
				});
		AddMetaData(world.component<glm::vec2>());
		AddMetaData(world.component<glm::vec3>());
		AddMetaData(world.component<glm::vec4>());
		AddMetaData(world.component<Components::TransformComponent>());
		AddMetaData(world.component<Components::SpriteComponent>());
		AddMetaData(world.component<Components::Camera2DComponent>());
		world.observer<Components::Camera2DComponent>().event(flecs::OnAdd).each([](flecs::entity e, Components::Camera2DComponent& c2dc) 
			{
				e.add<Components::TransformComponent>();
				c2dc.renderTarget = LoadRenderTexture((int)c2dc.renderTargetSize.x, (int)c2dc.renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
				SetTextureFilter(c2dc.renderTarget.texture, TEXTURE_FILTER_BILINEAR);
				c2dc.camera.zoom = c2dc.zoom;
			
			});
		world.observer<Components::Camera2DComponent>().event(flecs::OnRemove).each([](flecs::entity e, Components::Camera2DComponent& c2dc)
			{
				UnloadRenderTexture(c2dc.renderTarget);
			});

		world.observer<Components::SpriteComponent>().event(flecs::OnAdd).each([](flecs::entity e, Components::SpriteComponent& sp)
			{
				e.add<Components::TransformComponent>();
			});
		
		world.component<_Components::StartPhase>();
		world.component<_Components::PreUpdatePhase>();
		world.component<_Components::UpdatePhase>();
		world.component<_Components::PostUpdatePhase>();
		world.component<_Components::RenderPhase>();
		world.component<_Components::SceneEntityTag>();

		OnSharedLibraryEntry(world);
		//register builtin systems.
		sceneSystems["TransformSystem"] = world.system<Components::TransformComponent>("TransformSystem").each(Systems::TransformSystem);
		sceneSystems["TransformSystem"].add<_Components::PreUpdatePhase>();
		sceneSystems["Sprite2DRenderSystem"] = world.system<Components::TransformComponent, Components::SpriteComponent>("Sprite2DRenderSystem").each(Systems::Sprite2DRenderSystem);
		sceneSystems["Sprite2DRenderSystem"].add<_Components::RenderPhase>();
		sceneSystems["Camera2DTransformSystem"] = world.system<Components::TransformComponent, Components::Camera2DComponent>("Camera2DTransformSystem").each(Systems::Camera2DTransformSystem);
		sceneSystems["Camera2DTransformSystem"].add<_Components::PostUpdatePhase>();
		//register project components & systems.

		flecs::query regComponents = world.query<flecs::Component>();

		regComponents.each([&](flecs::entity e, flecs::Component& comp)
			{
				std::string path = e.path().c_str();
				if (!strstr(path.c_str(), "::flecs::") &&
					!strstr(path.c_str(), "::_Components::") &&
					!strstr(path.c_str(), "::std::") &&
					!strstr(path.c_str(), "::glm::") &&
					!strstr(path.c_str(), "::Systems::"))
				{
					componentsTable[e.name().c_str()] = e;
				}
			});

		flecs::query regSystems = world.query_builder().with(flecs::System).build();

		regSystems.each([&](flecs::entity e) 
			{
				std::string path = e.path().c_str();
				if (!strstr(path.c_str(), "::flecs::"))
				{
					systemsTable[e.name().c_str()] = e;
				}
			});
	}
	Scene::~Scene()
	{
		
	}
	void Scene::Start()
	{
		world.defer_begin();
		for (auto system : sceneSystems) 
		{
			if (system.second.has<_Components::StartPhase>())
			{
				flecs::system* s = (flecs::system*) & system.second;
				s->run();
			}
		}
		world.defer_end();
	}
	void Scene::Update()
	{
		world.defer_begin();

		for (auto system : sceneSystems)
		{
			if (system.second.has<_Components::PreUpdatePhase>())
			{
				flecs::system* s = (flecs::system*)&system.second;
				s->run();
			}
		}
		world.defer_end();

		world.defer_begin();

		for (auto system : sceneSystems)
		{
			if (system.second.has<_Components::UpdatePhase>())
			{
				flecs::system* s = (flecs::system*)&system.second;
				s->run();
			}
		}
		world.defer_end();

		world.defer_begin();

		for (auto system : sceneSystems)
		{
			if (system.second.has<_Components::PostUpdatePhase>())
			{
				flecs::system* s = (flecs::system*)&system.second;
				s->run();
			}
		}
		world.defer_end();

	}
	void Scene::Render()
	{
		

		if (Engine::GetSingleton()->GetSceneManager()->mode == SceneMode::Editor)
		{
			flecs::entity ts = sceneSystems["TransformSystem"];
			flecs::system* tss = (flecs::system*) & ts;
			tss->run();

			ts = sceneSystems["Camera2DTransformSystem"];
			tss = (flecs::system*)&ts;

			tss->run();
		}

		flecs::query cameras2D = world.query<Components::Camera2DComponent>();

		cameras2D.each([&](flecs::entity e, Components::Camera2DComponent& cc)
			{

				BeginTextureMode(cc.renderTarget);
				BeginMode2D(cc.camera);
				Color c;
				ClearBackground(c = GLMVec4ToRayColor(cc.backgroundColor));
				for (auto system : sceneSystems)
				{
					if (system.second.has<_Components::RenderPhase>())
					{
						flecs::system* s = (flecs::system*)&system.second;
						s->run();
					}
				}

				EndMode2D();
				EndTextureMode();
			});
	}
	void Scene::SetMainCamera(flecs::entity entity)
	{
		flecs::query cameras = world.query<Components::Camera2DComponent>();

		cameras.each([&](flecs::entity e, Components::Camera2DComponent& cc)
			{
				if (e == entity)
				{
					cc.isMain = true;
				}
				else 
				{
					cc.isMain = false;
				}
			});
	}
	void Scene::AddSystem(std::string name)
	{
		flecs::entity system = world.lookup(name.c_str());
		VE_ASSERT(system.id());
		sceneSystems[name.c_str()] = system;
	}
	flecs::entity Scene::AddEntity(std::string name)
	{
		std::string nameIndex = name + std::to_string(entityIndexGen);
		entityIndexGen++;
		return world.entity().set_name(nameIndex.c_str()).add<_Components::SceneEntityTag>();
	}
}