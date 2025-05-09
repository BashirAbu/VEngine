#include "ve_scene.h"
#include "ve_engine.h"
#include "components/ve_components.h"
#include "systems/ve_systems.h"
#include "utils/ve_utils.h"
#include <thread>
#include <rlgl.h>
#include <config.h>
void EngineGeneratedRegistration();

namespace VE
{
	Scene* Scene::singleton = nullptr;
	Scene::Scene(SceneType type, std::string flecsJson) : renderer(this)
	{
		singleton = this;
		sceneType = type;
#ifdef VE_EDITOR
		VE::Engine::GetSingleton()->LoadProjectSharedLibrary();
#endif

		//register builtin components.
		ManualComponentRegisteration();

		OnRender = world.entity("OnRender").add(flecs::PostUpdate);

		EngineGeneratedRegistration();
		OnSharedLibraryEntry(world);
		//register builtin systems.
		world.system<Components::TransformComponent, Components::SpriteComponent>("Sprite2DRenderSystem").kind(OnRender).without<_Components::Disabled>().each(Systems::Sprite2DRenderSystem);
		world.system<Components::TransformComponent, Components::Model3DComponent>("Mesh3DRenderSystem").kind(OnRender).without<_Components::Disabled>().each(Systems::Mesh3DRenderSystem);
		world.system<_Components::LightTag>("LightRenderSystem").kind(OnRender).without<_Components::Disabled>().run(Systems::LightRenderSystem);
		world.system<Components::TransformComponent, Components::Camera2DComponent>("Camera2DSystem").kind(flecs::PostUpdate).without<_Components::Disabled>().each(Systems::Camera2DSystem);
		world.system<Components::TransformComponent, Components::Camera3DComponent>("Camera3DSystem").kind(flecs::PostUpdate).without<_Components::Disabled>().each(Systems::Camera3DSystem);
		world.system<Components::TransformComponent>("TransformSystem").multi_threaded().kind(flecs::PreUpdate).without<_Components::Disabled>().each(Systems::TransformSystem);


		world.system<Components::UI::UICanvasComponent>("UICanvasSystem").kind(flecs::PostUpdate).without<_Components::Disabled>().each(Systems::UICanvasSystem);
		world.system<Components::TransformComponent, Components::UI::UILabelComponent>("UILabelRenderSystem").kind(OnRender).without<_Components::Disabled>().each(Systems::UILabelRenderSystem);
		world.system<Components::TransformComponent, Components::UI::UIImageComponent>("UIImageRenderSystem").kind(OnRender).without<_Components::Disabled>().each(Systems::UIImageRenderSystem);
		world.system<Components::TransformComponent, Components::UI::UIButtonComponent, Components::UI::UIImageComponent>("UIButtonSystem").kind(OnRender).without<_Components::Disabled>().each(Systems::UIButtonSystem);


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
		world.defer_begin();
		regSystems.each([&](flecs::entity e)
			{
				std::string path = e.path().c_str();
				if (!strstr(path.c_str(), "::flecs::"))
				{
					systemsTable[e.name().c_str()].entity = e;
					systemsTable[e.name().c_str()].enable = false;

					flecs::system* s = (flecs::system*)&e;
					s->disable();
				}
			});
		world.defer_end();
		//set number of threads;
		uint32_t numberOfThreads = std::thread::hardware_concurrency();
		world.set_threads(numberOfThreads);

		updatePipeline = world.pipeline().with(flecs::System).without(OnRender).build().set_name("UpdatePipeline");

		renderPipeline = world.pipeline().with(flecs::System).with(OnRender).build().set_name("RenderPipeline");

	}
	Scene::~Scene()
	{
		world.release();
#ifdef VE_EDITOR
		VE::Engine::GetSingleton()->UnloadProjectSharedLibrary();
#endif

		singleton = nullptr;
	}
	void Scene::Start()
	{
		flecs::entity transformSystem = flecs::entity();

		flecs::query q = world.query_builder().with(flecs::System).build();

		transformSystem = q.find([](flecs::entity e)
			{
				return (std::string)"TransformSystem" == e.name().c_str();
			});

		flecs::system* system = (flecs::system*)&transformSystem;
		system->run();

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
		flecs::query cameras = world.query_builder().with<Components::Camera2DComponent>().optional().with<Components::Camera3DComponent>().optional().build();

		cameras.each([&](flecs::entity e)
			{
				Components::Camera2DComponent* camera2D = nullptr;
				Components::Camera3DComponent* camera3D = nullptr;

				if (e.has<Components::Camera2DComponent>())
				{
					camera2D = e.get_mut<Components::Camera2DComponent>();
				}
				else if (e.has<Components::Camera3DComponent>())
				{
					camera3D = e.get_mut<Components::Camera3DComponent>();
				}

				if (e == entity)
				{
					if (camera2D)
					{
						camera2D->isMain = true;
					}
					else if (camera3D)
					{
						camera3D->isMain = true;
					}
				}
				else
				{
					if (camera2D)
					{
						camera2D->isMain = false;
					}
					else if (camera3D)
					{
						camera3D->isMain = false;
					}
				}
			});
	}
	flecs::entity Scene::GetMainCamera()
	{
		flecs::entity mainCamera = flecs::entity();

		flecs::query cameras = world.query_builder().with<Components::Camera2DComponent>().oper(flecs::Or).with<Components::Camera3DComponent>().build();

		mainCamera = cameras.find([&](flecs::entity e)
			{
				Components::Camera2DComponent* camera2D = nullptr;
				Components::Camera3DComponent* camera3D = nullptr;

				if (e.has<Components::Camera2DComponent>())
				{
					camera2D = e.get_mut<Components::Camera2DComponent>();
				}
				else if (e.has<Components::Camera3DComponent>())
				{
					camera3D = e.get_mut<Components::Camera3DComponent>();
				}

				if (camera2D)
				{
					return camera2D->isMain && !e.has<_Components::Disabled>();
				}
				else
				{
					return camera3D->isMain && !e.has<_Components::Disabled>();
				}
			});

		return mainCamera;
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
				std::string cloneChildName = GenUniqueName((std::string)child.name().c_str() + "Clone");
				cloneChild.set_name(cloneChildName.c_str());
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
		flecs::entity e = _LookupEntity(name);

		if (e)
		{
			e = e.has<_Components::Disabled>() ? flecs::entity() : e;
		}

		return e;
	}

	flecs::entity Scene::LookupEntity(flecs::entity id)
	{
		flecs::entity e = _LookupEntity(id);

		if (e)
		{
			e = e.has<_Components::Disabled>() ? flecs::entity() : e;
		}

		return e;
	}

	void Scene::EnableEntity(flecs::entity e)
	{
		std::function<void(flecs::entity child, bool enable)> enableChildren;
		enableChildren = [&](flecs::entity e, bool enable) -> void
			{

				if (!enable)
				{
					e.add< _Components::Disabled>();
				}
				else
				{
					e.remove< _Components::Disabled>();
				}
				e.children([&](flecs::entity c)
					{
						if (!enable)
						{
							c.add<_Components::Disabled>();
						}
						else
						{
							c.remove< _Components::Disabled>();
						}

						enableChildren(c, enable);
					});
			};


		if (e.has<_Components::Disabled>())
		{
			e.world().defer_begin();
			enableChildren(e, true);
			e.world().defer_end();

		}
	}

	void Scene::DisableEntity(flecs::entity e)
	{
		std::function<void(flecs::entity child, bool enable)> enableChildren;
		enableChildren = [&](flecs::entity e, bool enable) -> void
			{

				if (!enable)
				{
					e.add< _Components::Disabled>();
				}
				else
				{
					e.remove< _Components::Disabled>();
				}
				e.children([&](flecs::entity c)
					{
						if (!enable)
						{
							c.add<_Components::Disabled>();
						}
						else
						{
							c.remove< _Components::Disabled>();
						}

						enableChildren(c, enable);
					});
			};


		if (!e.has<_Components::Disabled>())
		{
			e.world().defer_begin();
			enableChildren(e, false);
			e.world().defer_end();

		}
	}

	flecs::entity Scene::_LookupEntity(std::string name)
	{
		flecs::entity ent = flecs::entity();

		auto q = world.query_builder().with<_Components::SceneEntityTag>().build();
		ent = q.find([&](flecs::entity e)
			{
				return name == e.name().c_str();
			});

		return ent;
	}
	//You can use int for id.
	flecs::entity Scene::_LookupEntity(flecs::entity id)
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
		if (!_LookupEntity(name))
		{
			return name;
		}
		else
		{
			std::string uniqueName = name;
			std::string temp = name;
			static size_t entityIDGen = 0;
			while (_LookupEntity(temp))
			{
				temp = uniqueName + std::to_string(entityIDGen++);
			}
			uniqueName = temp;
			return uniqueName;
		}
	}

	flecs::entity Scene::AddConstruct(std::filesystem::path constructFilePath)
	{
		std::string constructJsonString = "";
#ifdef VE_EDITOR
		std::filesystem::path fullpath = GetFullPath(constructFilePath).generic_string();

		std::fstream constructFile(fullpath);

		std::stringstream ss;
		ss << constructFile.rdbuf();
		constructJsonString = ss.str();
#else
		constructJsonString = VE::AssetsManager::GetSingleton()->LoadConstruct(constructFilePath);
#endif

		nlohmann::ordered_json constJson = nlohmann::ordered_json::parse(constructJsonString);
		std::string rootName = constJson["root"];

		//Check if entity with same name as construct exist.
		flecs::entity root = _LookupEntity(rootName);

		flecs::entity existingEnt = flecs::entity();

		//Entity with same name exist.
		if (root)
		{
			existingEnt = root;
			existingEnt.set_name((rootName + "_").c_str());
		}

		struct Node
		{
			flecs::entity e;
			std::string name;
		};

		std::vector<Node> nodes;

		for (auto ent : constJson["entities"])
		{
			Node n;
			flecs::entity e = world.entity();
			std::string entJsonString = ent.dump();
			e.from_json(entJsonString.c_str());
			n.e = e;
			n.name = ent["name"];
			nodes.push_back(n);
		}


		for (auto itr = constJson["relations"].begin(); itr != constJson["relations"].end(); itr++)
		{
			std::string childName = itr.key();
			std::string parentName = itr.value();

			auto it = std::find_if(nodes.begin(), nodes.end(), [&](const Node& node)
				{
					return node.name == childName;
				});
			Node child = *it;

			it = std::find_if(nodes.begin(), nodes.end(), [&](const Node& node)
				{
					std::string nodeName = node.name;
					return nodeName == parentName;
				});
			Node parent = *it;
			child.e.child_of(parent.e);
		}

		auto it = std::find_if(nodes.begin(), nodes.end(), [&](const Node& node)
			{
				return node.name == rootName;
			});
		Node rootNode = *it;
		root = rootNode.e;

		if (existingEnt)
		{
			root.set_name(GenUniqueName(rootName + "_").c_str());
			existingEnt.set_name(rootName.c_str());
		}
		return root;
	}





	//Serialization stuff
	void Scene::ManualComponentRegisteration()
	{
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
					std::string temp = data->generic_string();
					const char* str = temp.c_str();
					return s->value(flecs::String, &str);
				})
			.assign_string([](std::filesystem::path* data, const char* value)
				{
					*data = (std::string)value;
				});


		world.component<Texture>().member<flecs::u32_t>("id").member<flecs::i32_t>("width").member<flecs::i32_t>("height").member<flecs::i32_t>("mipmaps").member<flecs::i32_t>("format");

		world.component<std::vector<int8_t>>()
			.opaque(std_vector_support<int8_t>);

		world.component<std::vector<int16_t>>()
			.opaque(std_vector_support<int16_t>);

		world.component<std::vector<int32_t>>()
			.opaque(std_vector_support<int32_t>);

		world.component<std::vector<int64_t>>()
			.opaque(std_vector_support<int64_t>);


		world.component<std::vector<uint8_t>>()
			.opaque(std_vector_support<uint8_t>);

		world.component<std::vector<uint16_t>>()
			.opaque(std_vector_support<uint16_t>);

		world.component<std::vector<uint32_t>>()
			.opaque(std_vector_support<uint32_t>);

		world.component<std::vector<uint64_t>>()
			.opaque(std_vector_support<uint64_t>);



		world.component<std::vector<float>>()
			.opaque(std_vector_support<float>);

		world.component<std::vector<double>>()
			.opaque(std_vector_support<double>);

		world.component<std::vector<std::string>>()
			.opaque(std_vector_support<std::string>);

		world.component<std::vector<std::filesystem::path>>()
			.opaque(std_vector_support<std::filesystem::path>);



		world.component<glm::vec2>().member<float>("x").member<float>("y");

		world.component<std::vector<glm::vec2>>()
			.opaque(std_vector_support<glm::vec2>);


		world.component<glm::vec3>().member<float>("x").member<float>("y").member<float>("z");

		world.component<std::vector<glm::vec3>>()
			.opaque(std_vector_support<glm::vec3>);

		world.component<glm::vec4>().member<float>("x").member<float>("y").member<float>("z").member<float>("w");

		world.component<std::vector<glm::vec4>>()
			.opaque(std_vector_support<glm::vec4>);


		world.component<Components::TransformComponent>()
			.on_set([](flecs::entity e, Components::TransformComponent& tc)
				{
					tc.e = e;
				})
			.on_add([](flecs::entity e, Components::TransformComponent& tc)
				{
					tc.e = e;
				});

		world.component<Components::SpriteComponent>().on_add([](flecs::entity e, Components::SpriteComponent& sc)
			{
				e.add<Components::TransformComponent>();
			})
			.on_set([](flecs::entity e, Components::SpriteComponent& sc)
				{
					sc.texture = nullptr;
					if (!sc.texturePath.empty())
					{
						sc.texture = AssetsManager::GetSingleton()->LoadTexture(sc.texturePath);
					}
					sc.shader = nullptr;
					if (!sc.shaderPath.empty())
					{
						sc.shader = AssetsManager::GetSingleton()->LoadShader(sc.shaderPath);
					}
				});

		world.component<Components::Camera2DComponent>().on_add([](flecs::entity e, Components::Camera2DComponent& c2dc)
			{
				e.add<Components::TransformComponent>();
				c2dc.renderTarget = LoadRenderTexture((int)c2dc.renderTargetSize.x, (int)c2dc.renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
				SetTextureFilter(c2dc.renderTarget.texture, TEXTURE_FILTER_BILINEAR);
				c2dc.camera.zoom = c2dc.zoom;

			}).on_remove([](flecs::entity e, Components::Camera2DComponent& c2dc)
				{
					UnloadRenderTexture(c2dc.renderTarget);
				});
			world.component<Components::Camera3DComponent>().on_set([](flecs::entity e, Components::Camera3DComponent& c3dc)
				{
					c3dc.camera.fovy = 45.0f;
					c3dc.camera.projection = CAMERA_PERSPECTIVE;
					c3dc.camera.target = {};
					c3dc.camera.up = { 0.0f, 1.0f, 1.0f };

					e.add<Components::TransformComponent>();
					c3dc.renderTarget = LoadRenderTexture((int)c3dc.renderTargetSize.x, (int)c3dc.renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
					SetTextureFilter(c3dc.renderTarget.texture, TEXTURE_FILTER_BILINEAR);

					c3dc.skyboxTexture = {};
					c3dc.cubeMapModel = {};
					c3dc.skyboxShader = nullptr;

					Components::LoadSkymap(&c3dc);

				}).on_remove([](flecs::entity e, Components::Camera3DComponent& c3dc)
					{
						UnloadRenderTexture(c3dc.renderTarget);
					});

				world.component<Components::DirectionalLightComponent>().on_add([](flecs::entity e, Components::DirectionalLightComponent& light)
					{
						e.add<_Components::LightTag>();
						light.pbrShader = VE::AssetsManager::GetSingleton()->LoadShader("shaders/pbr.glsl");
					});
				world.component<Components::PointLightComponent>().on_add([](flecs::entity e, Components::PointLightComponent& light)
					{
						e.add<_Components::LightTag>();
						light.pbrShader = VE::AssetsManager::GetSingleton()->LoadShader("shaders/pbr.glsl");
					});
				world.component<Components::SpotLightComponent>().on_add([](flecs::entity e, Components::SpotLightComponent& light)
					{
						e.add<_Components::LightTag>();
						light.pbrShader = VE::AssetsManager::GetSingleton()->LoadShader("shaders/pbr.glsl");
					});


				world.component<Components::Model3DComponent>().on_add([](flecs::entity e, Components::Model3DComponent& model)
					{

					})
					.on_set([](flecs::entity e, Components::Model3DComponent& model)
						{

							if (model.model)
							{
								UnloadModel(*model.model);
								if (model.basicMesh != _Components::BasicMesh::None)
								{
									free(model.model);
								}
							}
							model.model = nullptr;

							if (!model.modelFilepath.empty())
							{
								model.model = VE::AssetsManager::GetSingleton()->LoadModel(model.modelFilepath);
							}
							if (model.basicMesh != _Components::BasicMesh::None)
							{
								Model* basicModel = (Model*)malloc(sizeof(Model));
								*basicModel = Components::LoadBasicMesh(model.basicMesh);
								model.model = basicModel;
							}

							Shader* pbrShader = AssetsManager::GetSingleton()->LoadShader("shaders/pbr.glsl");


							int loc = rlGetLocationAttrib(pbrShader->id, "vertexPosition");
							pbrShader->locs[RL_SHADER_LOC_VERTEX_POSITION] = loc;

							loc = rlGetLocationAttrib(pbrShader->id, "vertexTexCoord");
							pbrShader->locs[RL_SHADER_LOC_VERTEX_TEXCOORD01] = loc;

							loc = rlGetLocationAttrib(pbrShader->id, "vertexNormal");
							pbrShader->locs[RL_SHADER_LOC_VERTEX_NORMAL] = loc;


							loc = rlGetLocationUniform(pbrShader->id, "modelMatrix");
							pbrShader->locs[SHADER_LOC_MATRIX_MODEL] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "mvp");
							pbrShader->locs[RL_SHADER_LOC_MATRIX_MVP] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "material.albedo.color");
							pbrShader->locs[RL_SHADER_LOC_COLOR_DIFFUSE] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "albedoTexture");
							pbrShader->locs[RL_SHADER_LOC_MAP_DIFFUSE] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "specularTexture");
							pbrShader->locs[RL_SHADER_LOC_MAP_SPECULAR] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "material.specular.color");
							pbrShader->locs[RL_SHADER_LOC_COLOR_SPECULAR] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "ambientOcclusionTexture");
							pbrShader->locs[RL_SHADER_LOC_MAP_OCCLUSION] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "material.ambientOcclusion.color");
							pbrShader->locs[RL_SHADER_LOC_COLOR_AMBIENT] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "normalMatrix");
							pbrShader->locs[RL_SHADER_LOC_MATRIX_NORMAL] = loc;

							loc = rlGetLocationUniform(pbrShader->id, "projectionMatrix");
							pbrShader->locs[RL_SHADER_LOC_MATRIX_PROJECTION] = loc;



							Components::LoadModelMaterials(model, pbrShader);

						}).on_remove([](flecs::entity e, Components::Model3DComponent& model)
							{
								if (model.basicMesh != _Components::BasicMesh::None)
								{
									UnloadModel(*model.model);
									free(model.model);
									model.model = nullptr;
								}
							});


						world.component<Components::UI::UILabelComponent>().on_set([](flecs::entity e, Components::UI::UILabelComponent& lb)
							{
								lb.texturePtr = nullptr;
								lb.texture = {};
								lb.font = nullptr;

								if (!lb.fontFilepath.empty())
								{
									lb.font = AssetsManager::GetSingleton()->LoadFont(lb.fontFilepath, (int)lb.size);
									lb.texture = RaylibGetTextureFromText_UTF8(lb.font, lb.text);
									lb.texturePtr = &lb.texture;
								}
							})
							.on_remove([](flecs::entity e, Components::UI::UILabelComponent& lb)
								{
									if (lb.texturePtr)
									{
										UnloadTexture(*lb.texturePtr);
									}
									if (lb.font)
									{
										delete lb.font;
										lb.font = nullptr;
									}
								});

						world.component<Components::UI::UIImageComponent>().on_set([](flecs::entity e, Components::UI::UIImageComponent& img)
							{
								img.texture = nullptr;
								if (!img.imageFilepath.empty())
								{
									img.texture = AssetsManager::GetSingleton()->LoadTexture(img.imageFilepath);
								}
							});

						world.component<Components::UI::UIButtonComponent>().on_add([](flecs::entity e, Components::UI::UIButtonComponent)
							{
								e.add<Components::UI::UIImageComponent>();
							}).on_set([](flecs::entity e, Components::UI::UIButtonComponent)
								{
									Components::UI::UIImageComponent& img = *e.get_mut<Components::UI::UIImageComponent>();

									img.texture = nullptr;

									if (!img.imageFilepath.empty())
									{
										img.texture = AssetsManager::GetSingleton()->LoadTexture(img.imageFilepath);
									}

								});

							world.component<_Components::SceneEntityTag>();
							world.component<_Components::SceneEntityUITag>();
	}
}