#include "ve_scene.h"
#include "ve_engine.h"
#include "editor/ve_editor_input.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include <imgui.h>
#include <flecs/addons/meta.h>

namespace VE 
{
	Scene::Scene(SceneType type)
	{
		sceneType = type;

		//register builtin components.
		world.component<Components::TransformComponent>();
		world.component<Components::SpriteComponent>();
		//system phases
		
		world.component<_Components::StartPhase>();
		world.component<_Components::PreUpdatePhase>();
		world.component<_Components::UpdatePhase>();
		world.component<_Components::PostUpdatePhase>();
		world.component<_Components::RenderPhase>();

		//register builtin systems.
		sceneSystems["TransformSystem"] = world.system<Components::TransformComponent>("TransformSystem").with<_Components::PreUpdatePhase>().each(Systems::TransformSystem);
		sceneSystems["Renderer2DSystem"] = world.system<Components::TransformComponent, Components::SpriteComponent>("Renderer2DSystem").with<_Components::StartPhase>().run(Systems::Sprite2DRenderSystem);
		//register project components & systems.
		OnSharedLibraryEntry(world);


		flecs::query regComponents = world.query<flecs::Component>();

		regComponents.each([&](flecs::entity e, flecs::Component& comp)
			{
				std::string path = e.path().c_str();
				if (path.find("::flecs::") != 0)
				{
					if(path.find("::VE::_Components::") != 0)
						componentsTable[e.name().c_str()] = e;
				}
			});

		flecs::query regSystems = world.query_builder().with(flecs::System).build();

		regSystems.each([&](flecs::entity e)
			{
				std::string path = e.path().c_str();
				if (path.find("::flecs::") != 0)
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
		for (auto system : sceneSystems)
		{
			if (system.second.has<_Components::RenderPhase>())
			{
				flecs::system* s = (flecs::system*)&system.second;
				s->run();
			}
		}
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
		return world.entity().set_name(nameIndex.c_str()).add<_Components::SceneTag>();
	}
}