#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "components/ve_components.h"
using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;
void Serialize_TheCompo()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "TheCompo"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::DOG::TheCompo>* comp = (flecs::component<VE::Components::DOG::TheCompo>*)&compEntity; 
		comp->opaque(comp->world().component().member<float>("age").member<std::string>("name").member<glm::vec3>("postion"))
		.serialize([](const flecs::serializer* s, const VE::Components::DOG::TheCompo* data) -> int		{
		s->member("age");
		s->value(data->age);
		s->member("name");
		s->value(data->name);
		s->member("postion");
		s->value(data->postion);
			 return 0;
		}).ensure_member([](VE::Components::DOG::TheCompo* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "age")) { return &data->age;}
			else if (!strcmp(member, "name")) { return &data->name;}
			else if (!strcmp(member, "postion")) { return &data->postion;}
		return nullptr;
		});
	}
}


void Serialize_PlayerMovement()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "PlayerMovement"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::PlayerMovement>* comp = (flecs::component<VE::Components::PlayerMovement>*)&compEntity; 
		comp->opaque(comp->world().component().member<float>("speed").member<float>("rotationSpeed").member<glm::vec3>("velocity").member<glm::vec3>("direction"))
		.serialize([](const flecs::serializer* s, const VE::Components::PlayerMovement* data) -> int		{
		s->member("speed");
		s->value(data->speed);
		s->member("rotationSpeed");
		s->value(data->rotationSpeed);
		s->member("velocity");
		s->value(data->velocity);
		s->member("direction");
		s->value(data->direction);
			 return 0;
		}).ensure_member([](VE::Components::PlayerMovement* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "speed")) { return &data->speed;}
			else if (!strcmp(member, "rotationSpeed")) { return &data->rotationSpeed;}
			else if (!strcmp(member, "velocity")) { return &data->velocity;}
			else if (!strcmp(member, "direction")) { return &data->direction;}
		return nullptr;
		});
	}
}


void EngineGeneratedSerialization()
{
	 Serialize_TheCompo();
	 Serialize_PlayerMovement();
}




void EngineGeneratedRegistration()
{
	 VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::DOG::TheCompo>();
	 VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::PlayerMovement>();
	 VE::Scene::GetSingleton()->GetFlecsWorld().system<TransformComponent,SpriteComponent>().multi_threaded().each(VE::Components::testSystem);
}
