#include "ve_scene.h"
#include "ve_engine.h"
#include "editor/ve_editor_input.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include "utils/ve_utils.h"
#include <imgui.h>
#include <thread>
namespace VE 
{
	Scene* Scene::singleton = nullptr;
	Scene::Scene(SceneType type, std::string flecsJson) : renderer(this)
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
		AddMetaData(world.component<Components::SpriteComponent>().on_add([](flecs::entity e, Components::SpriteComponent& sp)
			{
				e.add<Components::TransformComponent>();
			})
			.on_set([](flecs::entity e, Components::SpriteComponent& sp)
				{
					e.add<Components::TransformComponent>();
				}));
		AddMetaData(world.component<Components::Camera2DComponent>().on_add([](flecs::entity e, Components::Camera2DComponent& c2dc)
			{
				e.add<Components::TransformComponent>();
				c2dc.renderTarget = LoadRenderTexture((int)c2dc.renderTargetSize.x, (int)c2dc.renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
				SetTextureFilter(c2dc.renderTarget.texture, TEXTURE_FILTER_BILINEAR);
				c2dc.camera.zoom = c2dc.zoom;

			}).on_remove([](flecs::entity e, Components::Camera2DComponent& c2dc)
				{
					UnloadRenderTexture(c2dc.renderTarget);
				}));
		world.component<_Components::SceneEntityTag>();
		
		OnRender = world.entity("OnRender").add(flecs::PostUpdate);

		OnSharedLibraryEntry(world);
		//register builtin systems.
		world.system<Components::TransformComponent, Components::SpriteComponent>("Sprite2DRenderSystem").kind(OnRender).multi_threaded().each(Systems::Sprite2DRenderSystem);
		world.system<Components::TransformComponent, Components::Camera2DComponent>("Camera2DTransformSystem").kind(flecs::PostUpdate).each(Systems::Camera2DTransformSystem);
		world.system<Components::TransformComponent>("TransformSystem").multi_threaded().kind(flecs::PreUpdate).each(Systems::TransformSystem);
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
					systemsTable[e.name().c_str()].entity = e;
					systemsTable[e.name().c_str()].enable = false;
				}
			});

		//set number of threads;
		uint32_t numberOfThreads = std::thread::hardware_concurrency();
		world.set_threads(numberOfThreads);

		updatePipeline = world.pipeline().with(flecs::System).without(OnRender).build().set_name("UpdatePipeline");
			
		renderPipeline = world.pipeline().with(flecs::System).with(OnRender).build().set_name("RenderPipeline");

	}
	Scene::~Scene()
	{
		world.reset();
		VE::Engine::GetSingleton()->UnloadProjectSharedLibrary();
		singleton = nullptr;
	}
	void Scene::Start()
	{
	}
	void Scene::Update()
	{
		renderer.BeginFrame();
		world.set_pipeline(updatePipeline);

		world.progress();

		//deferred
		for (auto it = deferredConstructs.begin(); it != deferredConstructs.end();)
		{
			AddConstruct(*it);
			it = deferredConstructs.erase(it);
		}
	}
	void Scene::Render()
	{
		//call RenderScene
		renderer.RenderScene();
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
		flecs::entity e = world.entity().set_name(GenUniqueName(name).c_str()).add<_Components::SceneEntityTag>().add<Components::TransformComponent>();
		return e;
	}

	std::string Scene::SerializeScene()
	{
		std::string sceneJson;

		flecs::query sceneEntities = world.query_builder().with<_Components::SceneEntityTag>().build();

		sceneEntities.each([&](flecs::entity e)
			{
				std::string entityJson = SerializeEntity(e);
				sceneJson += entityJson + ",";
			});
		if (!sceneJson.empty())
		{
			sceneJson.pop_back();
		}
		sceneJson = "{ \"results\":[" + sceneJson + "]}";

		return sceneJson;
	}

	std::string Scene::SerializeEntity(flecs::entity e)
	{
		std::string entityJson = {};

		bool hasChildren = false;

		e.children([&](flecs::entity c)
			{
				hasChildren = true;
				return;
			});

		if (hasChildren)
		{
			std::string parentJson = e.to_json().c_str();
			std::string childrenJson = {};
			e.children([&](flecs::entity child)
				{
					bool hasChildChildren = false;

					child.children([&](flecs::entity c)
						{
							hasChildChildren = true;
							return;
						});

					if (hasChildChildren)
					{
						
						std::function<void(flecs::entity, std::string&)> serializeChildren;

						serializeChildren = [&](flecs::entity child, std::string& entityJson)
							{
								child.children([&](flecs::entity child)
									{
										bool hasChildChildren = false;

										child.children([&](flecs::entity c)
											{
												hasChildChildren = true;
												return;
											});

										if (hasChildChildren)
										{
											serializeChildren(child, entityJson); 
										}
										entityJson += (std::string)child.to_json().c_str() + (std::string)",";
									});
							};

						serializeChildren(child, childrenJson);
					}
					childrenJson += (std::string)child.to_json().c_str() + (std::string)",";
				});

			if (!childrenJson.empty())
			{
				childrenJson.pop_back();
			}
			entityJson = parentJson + ", " + childrenJson;
		}
		else
		{
			entityJson = (std::string)e.to_json().c_str();
		}

		return entityJson;
	}

	void Scene::CloneChildren(flecs::entity entity, flecs::entity cloneParent)
	{
		world.defer_begin();
		entity.children([&](flecs::entity child)
			{
				flecs::entity cloneChild = child.clone(true);
				cloneChild.set_name(child.name());
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
		
		
		auto q = world.query_builder().with<_Components::SceneEntityTag>().build();
		ent = q.find([&](flecs::entity e)
			{
				return name == e.name().c_str();
			});
		
		return ent;
	}

	flecs::entity Scene::LookupEntity(flecs::entity id)
	{
		flecs::entity ent;
		auto q = world.query_builder().with<_Components::SceneEntityTag>().build();
		ent = q.find([&](flecs::entity e)
			{
				return (int)e == (int)id;
			});
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

			flecs::query findConstruct = world.query_builder().with<_Components::ConstructTag>().build();
			flecs::entity root = findConstruct.find([&](flecs::entity e) 
				{
					
					if (rootName == e.name().c_str())
					{
						rootName = e.name().c_str();
						return e;
					}
					return flecs::entity();
				});

			//if entity inside the scene has the same name as the construct we need to rename the construct's root entity.
			if (root)
			{
				root = CloneEntity(root);
			}
			else
			{
				//Check if entity with same name as construct exist.
				root = LookupEntity(rootName);
				//Entity with same name exist.
				if (root)
				{
					//Rename other entity.
					root.set_name(GenUniqueName((std::string)root.name().c_str() + "_").c_str());
				}
				world.from_json(constructJson.c_str());
				root = LookupEntity(rootName);
				root.add<_Components::ConstructTag>();
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