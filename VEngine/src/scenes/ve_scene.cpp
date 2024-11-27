#include "ve_scene.h"
#include "ve_engine.h"
#include "editor/ve_editor_input.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include "utils/ve_utils.h"
#include <imgui.h>
namespace VE 
{
	Scene* Scene::singleton = nullptr;
	Scene::Scene(SceneType type)
	{
		singleton = this;
		sceneType = type;

		VE::Engine::GetSingleton()->LoadProjectSharedLibrary();

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
		AddMetaData(world.component<Components::TransformComponent>()
			.on_set([](flecs::entity e, Components::TransformComponent & tc)
			{
				tc.e = e;
			})
			.on_add([](flecs::entity e, Components::TransformComponent& tc)
				{
					tc.e = e;
				}));
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
		world.reset();
		VE::Engine::GetSingleton()->UnloadProjectSharedLibrary();
		singleton = nullptr;
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
		flecs::entity e = world.lookup("StartPlayerSystem");
		flecs::system* s = (flecs::system*)&e;
		s->run();
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

		for (auto it = deferredConstructs.begin(); it != deferredConstructs.end();)
		{
			AddConstruct(*it);
			it = deferredConstructs.erase(it);
		}
	}
	void Scene::Render()
	{
		if (Engine::GetSingleton()->GetSceneManager()->mode == SceneMode::Editor)
		{
			flecs::entity ts = sceneSystems["Camera2DTransformSystem"];
			flecs::system* tss = (flecs::system*)&ts;

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
	flecs::entity Scene::AddEntity(std::string name)
	{
		flecs::entity e = world.entity().set_name(GenUniqueName(name).c_str()).add<_Components::SceneEntityTag>();
		cachedEntitiesTable[e.name().c_str()] = e;
		return e;
	}

	void Scene::CloneChildren(flecs::entity entity, flecs::entity cloneParent)
	{
		world.defer_begin();
		entity.children([&](flecs::entity child)
			{
				flecs::entity cloneChild = child.clone(true);
				cloneChild.set_name(child.name());
				cachedEntitiesTable[cloneChild.name().c_str()] = cloneChild;
				cloneChild.remove(flecs::ChildOf, entity);
				cloneChild.child_of(cloneParent);
				CloneChildren(child, cloneChild);
			});
		world.defer_end();

	}

	flecs::entity Scene::CloneEntity(flecs::entity entity)
	{
		flecs::entity clone = entity.clone(true);
		std::string cloneName = std::string(entity.name().c_str()) + "Clone";
		const flecs::entity_t* p = (flecs::entity_t*) & entity;
		clone.set_name(GenUniqueName(cloneName).c_str());
		cachedEntitiesTable[clone.name().c_str()] = clone;
		if (entity.parent())
		{	
			//use this line of you want duplicated entity to remove its parent.
			//clone.remove(flecs::ChildOf, entity.parent());
		}
		CloneChildren(entity, clone);
		return clone;
	}
	flecs::entity Scene::LookupEntity(std::string name)
	{
		flecs::entity ent = flecs::entity();
		
		if (cachedEntitiesTable.find(name) != cachedEntitiesTable.end())
		{
			ent = cachedEntitiesTable[name];
		}
		else 
		{
			auto q = world.query_builder().with(flecs::Wildcard).build();
			ent = q.find([&](flecs::entity e)
				{
					return name == e.name().c_str();
				});
			if (ent)
			{
				cachedEntitiesTable[name] = ent;
			}
		}
		return ent;
	}
	std::string Scene::GenUniqueName(std::string name)
	{
		if (!LookupEntity(name))
		{
			return name;
		}
		else 
		{
			std::string uniqueName = name;
			std::string temp = name;
			size_t entityIDGen = 0;
			while (LookupEntity(temp))
			{
				temp = uniqueName + std::to_string(entityIDGen++);
			}
			uniqueName = temp;
			return uniqueName;
		}
	}

	std::string Scene::AddConstruct(std::filesystem::path constructFilePath)
	{
		if (world.is_deferred())
		{
			std::fstream constructFile(GetFullPath(constructFilePath));
			std::stringstream ss;
			ss << constructFile.rdbuf();
			std::string constructJson = ss.str();

			nlohmann::ordered_json constJson = nlohmann::ordered_json::parse(constructJson);
			std::string rootName;
			for (auto ent : constJson["results"])
			{
				if (!ent.contains("parent"))
				{
					rootName = ent["name"];
					break;
				}
			}

			deferredConstructs.push_back(constructFilePath);
			return rootName;
		}
		else 
		{
			std::fstream constructFile(GetFullPath(constructFilePath));
			std::stringstream ss;
			ss << constructFile.rdbuf();
			std::string constructJson = ss.str();

			nlohmann::ordered_json constJson = nlohmann::ordered_json::parse(constructJson);
			std::string rootName;
			for (auto ent : constJson["results"])
			{
				if (!ent.contains("parent"))
				{
					rootName = ent["name"];
					break;
				}
			}

			flecs::entity root = LookupEntity(rootName);
			if (root)
			{
				root = CloneEntity(root);
			}
			else
			{
				world.from_json(constructJson.c_str());
				root = LookupEntity(rootName);
				cachedEntitiesTable[rootName] = root;
			}

			Components::TransformComponent* rootTC = root.get_mut<Components::TransformComponent>();
			if (rootTC)
			{
				rootTC->SetWorldPosition(glm::vec3(0.0f));
				rootTC->SetWorldRotation(glm::vec3(0.0f));
				rootTC->SetWorldScale(glm::vec3(0.0f));
			}
			return rootName;
		}
	}
}