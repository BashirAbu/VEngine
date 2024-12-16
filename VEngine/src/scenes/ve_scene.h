#pragma once
#include "ve_defines.h"
#include <glm/glm.hpp>
#include <raylib.h>
#include <filesystem>
#include <flecs.h>
#include <queue>
#include <mutex>
#include "ve_renderer.h"

#define MAX_LIGHTS 124

namespace VE 
{
	
	enum class SceneType 
	{
		Scene2D,
		Scene3D
	};
	class VE_API Scene
	{
	public:
		Scene(SceneType type, std::string flecsJson = "");
		~Scene();

		static Scene* GetSingleton() { return singleton; }

		void Start();
		void Update();
		void Render();

		void SetMainCamera(flecs::entity entity);
		flecs::entity GetMainCamera();
		flecs::entity AddEntity(std::string name);
		flecs::entity AddConstruct(std::filesystem::path constructFilePath);
		const SceneType GetSceneType() const { return sceneType; }
		void ManualComponentRegisteration();

		flecs::entity CloneEntity(flecs::entity entity);

		flecs::entity LookupEntity(std::string name);
		//You can use int for id.
		flecs::entity LookupEntity(flecs::entity id);

		void EnableEntity(flecs::entity e);
		void DisableEntity(flecs::entity e);

		//System render phase
		flecs::entity OnRender;
		flecs::entity updatePipeline;
		flecs::entity renderPipeline;

		std::string SerializeScene();
		std::string SerializeEntity(flecs::entity e);
		flecs::world GetFlecsWorld() { return world; }

		const std::filesystem::path GetScenePath() { return scenePath; }
		flecs::entity _LookupEntity(std::string name);
		//You can use int for id.
		flecs::entity _LookupEntity(flecs::entity id);

		RenderTexture GetMainRenderTarget() { return renderer.GetMainRenderTarget(); };
		Renderer renderer; 
	private:

		void CloneChildren(flecs::entity entity, flecs::entity cloneParent);
		std::string GenUniqueName(std::string name);
		flecs::world world;
		glm::vec4 clearColor;

		std::unordered_map<std::string, flecs::entity> componentsTable;

		struct SystemInfo 
		{
			flecs::entity entity;
			bool enable = false;
		};

		std::unordered_map<std::string, SystemInfo> systemsTable;
		
		SceneType sceneType;
		bool started = false;

		std::filesystem::path scenePath = "";

		std::vector<std::filesystem::path> deferredConstructs;

		friend class Editor;
		friend class SceneManager;
		friend class Renderer;
		static Scene* singleton;
	};

	template <typename Elem, typename Vector = std::vector<Elem>>
	inline flecs::opaque<Vector, Elem> std_vector_support(flecs::world& world) {
		return flecs::opaque<Vector, Elem>()
			.as_type(world.vector<Elem>())

			// Forward elements of std::vector value to serializer
			.serialize([](const flecs::serializer* s, const Vector* data) {
			for (const auto& el : *data) {
				s->value(el);
			}
			return 0;
				})

			// Return vector count
			.count([](const Vector* data) {
			return data->size();
				})

			// Resize contents of vector
			.resize([](Vector* data, size_t size) {
			data->resize(size);
				})

			// Ensure element exists, return pointer
			.ensure_element([](Vector* data, size_t elem) {
			if (data->size() <= elem) {
				data->resize(elem + 1);
			}

			return &data->data()[elem];
				});
	}
}