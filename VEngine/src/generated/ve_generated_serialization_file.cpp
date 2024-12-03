#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "components/ve_components.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;


void Serialize_TransformComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "TransformComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::TransformComponent>* comp = (flecs::component<VE::Components::TransformComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<glm::vec3>("localPosition").member<glm::vec3>("localRotation").member<glm::vec3>("localScale"))
		.serialize([](const flecs::serializer* s, const VE::Components::TransformComponent* data) -> int		{
		s->member("localPosition");
		s->value(data->localPosition);
		s->member("localRotation");
		s->value(data->localRotation);
		s->member("localScale");
		s->value(data->localScale);
			 return 0;
		}).ensure_member([](VE::Components::TransformComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "localPosition")) { return &data->localPosition;}
			else if (!strcmp(member, "localRotation")) { return &data->localRotation;}
			else if (!strcmp(member, "localScale")) { return &data->localScale;}
		return nullptr;
		});
	}
}


void Serialize_SpriteComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "SpriteComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::SpriteComponent>* comp = (flecs::component<VE::Components::SpriteComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<std::filesystem::path>("texturePath").member<glm::vec2>("origin").member<NormalizedColor>("tintColor").member<int32_t>("renderOrder"))
		.serialize([](const flecs::serializer* s, const VE::Components::SpriteComponent* data) -> int		{
		s->member("texturePath");
		s->value(data->texturePath);
		s->member("origin");
		s->value(data->origin);
		s->member("tintColor");
		s->value(data->tintColor);
		s->member("renderOrder");
		s->value(data->renderOrder);
			 return 0;
		}).ensure_member([](VE::Components::SpriteComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "texturePath")) { return &data->texturePath;}
			else if (!strcmp(member, "origin")) { return &data->origin;}
			else if (!strcmp(member, "tintColor")) { return &data->tintColor;}
			else if (!strcmp(member, "renderOrder")) { return &data->renderOrder;}
		return nullptr;
		});
	}
}


void Serialize_Camera2DComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "Camera2DComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::Camera2DComponent>* comp = (flecs::component<VE::Components::Camera2DComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<glm::vec2>("renderTargetSize").member<NormalizedColor>("backgroundColor").member<float>("zoom").member<bool>("isMain"))
		.serialize([](const flecs::serializer* s, const VE::Components::Camera2DComponent* data) -> int		{
		s->member("renderTargetSize");
		s->value(data->renderTargetSize);
		s->member("backgroundColor");
		s->value(data->backgroundColor);
		s->member("zoom");
		s->value(data->zoom);
		s->member("isMain");
		s->value(data->isMain);
			 return 0;
		}).ensure_member([](VE::Components::Camera2DComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "renderTargetSize")) { return &data->renderTargetSize;}
			else if (!strcmp(member, "backgroundColor")) { return &data->backgroundColor;}
			else if (!strcmp(member, "zoom")) { return &data->zoom;}
			else if (!strcmp(member, "isMain")) { return &data->isMain;}
		return nullptr;
		});
	}
}


void EngineGeneratedSerialization()
{
	 Serialize_TransformComponent();
	 Serialize_SpriteComponent();
	 Serialize_Camera2DComponent();
}




void EngineGeneratedRegistration()
{
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::TransformComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::SpriteComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::Camera2DComponent>();
	EngineGeneratedSerialization();
}
