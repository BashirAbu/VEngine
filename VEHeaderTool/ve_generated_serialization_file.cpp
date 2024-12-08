#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "../../../VEProjects/testPrj/src/components.h"

#include "../../../VEProjects/testPrj/src/systems.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;


void Serialize_PlayerComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "PlayerComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<PlayerComponent>* comp = (flecs::component<PlayerComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<float>("speed").member<glm::vec3>("direction"))
		.serialize([](const flecs::serializer* s, const PlayerComponent* data) -> int		{
		s->member("speed");
		s->value(data->speed);
		s->member("direction");
		s->value(data->direction);
			 return 0;
		}).ensure_member([](PlayerComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "speed")) { return &data->speed;}
			else if (!strcmp(member, "direction")) { return &data->direction;}
		return nullptr;
		});
	}
}


void Serialize_EnemyComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "EnemyComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<EnemyComponent>* comp = (flecs::component<EnemyComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<float>("speed").member<float>("rotationSpeed"))
		.serialize([](const flecs::serializer* s, const EnemyComponent* data) -> int		{
		s->member("speed");
		s->value(data->speed);
		s->member("rotationSpeed");
		s->value(data->rotationSpeed);
			 return 0;
		}).ensure_member([](EnemyComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "speed")) { return &data->speed;}
			else if (!strcmp(member, "rotationSpeed")) { return &data->rotationSpeed;}
		return nullptr;
		});
	}
}


void Serialize_BulletComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "BulletComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<BulletComponent>* comp = (flecs::component<BulletComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<float>("speed"))
		.serialize([](const flecs::serializer* s, const BulletComponent* data) -> int		{
		s->member("speed");
		s->value(data->speed);
			 return 0;
		}).ensure_member([](BulletComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "speed")) { return &data->speed;}
		return nullptr;
		});
	}
}


void ProjectGeneratedSerialization()
{
	 Serialize_PlayerComponent();
	 Serialize_EnemyComponent();
	 Serialize_BulletComponent();
}




void ProjectGeneratedRegistration()
{
	VE::Scene::GetSingleton()->GetFlecsWorld().component<PlayerComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<EnemyComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<BulletComponent>().on_add(OnBulletAdd);
	ProjectGeneratedSerialization();
	VE::Scene::GetSingleton()->GetFlecsWorld().system<PlayerComponent>("StartPlayerSystem").kind(flecs::OnStart).each(StartPlayerSystem);
	VE::Scene::GetSingleton()->GetFlecsWorld().system<VE::Components::TransformComponent,PlayerComponent>("PlayerMovement").kind(flecs::OnUpdate).each(PlayerMovement);
	VE::Scene::GetSingleton()->GetFlecsWorld().system<VE::Components::TransformComponent,EnemyComponent>("EnemyBehavior").kind(flecs::OnUpdate).multi_threaded().each(EnemyBehavior);
	VE::Scene::GetSingleton()->GetFlecsWorld().system<VE::Components::TransformComponent,BulletComponent>("BulletMover").kind(flecs::OnUpdate).each(BulletMover);
}
