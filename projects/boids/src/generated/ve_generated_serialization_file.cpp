#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "components/ve_components.h"

#include "components.h"

#include "systems.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::Components::UI;
using namespace VE::_Components;


void Serialize_ManagerComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "ManagerComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<ManagerComponent>* comp = (flecs::component<ManagerComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<float>("speed").member<float>("radius").member<float>("alignment").member<float>("seperation").member<float>("cohesion"))
		.serialize([](const flecs::serializer* s, const ManagerComponent* data) -> int		{
		s->member("speed");
		s->value(data->speed);
		s->member("radius");
		s->value(data->radius);
		s->member("alignment");
		s->value(data->alignment);
		s->member("seperation");
		s->value(data->seperation);
		s->member("cohesion");
		s->value(data->cohesion);
			 return 0;
		}).ensure_member([](ManagerComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "speed")) { return &data->speed;}
			else if (!strcmp(member, "radius")) { return &data->radius;}
			else if (!strcmp(member, "alignment")) { return &data->alignment;}
			else if (!strcmp(member, "seperation")) { return &data->seperation;}
			else if (!strcmp(member, "cohesion")) { return &data->cohesion;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<ManagerComponent>>().opaque(VE::std_vector_support<ManagerComponent>);
	}
}


void ProjectGeneratedSerialization()
{
	 Serialize_ManagerComponent();
}




void ProjectGeneratedRegistration()
{
	VE::Scene::GetSingleton()->GetFlecsWorld().component<ManagerComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<BoidComponent>().on_add(OnBoidComponentAdd);
	ProjectGeneratedSerialization();
	VE::Scene::GetSingleton()->GetFlecsWorld().system<VE::Components::TransformComponent,BoidComponent>("MovementSystem").kind(flecs::OnUpdate).without<_Components::Disabled>().each(MovementSystem);
	VE::Scene::GetSingleton()->GetFlecsWorld().system<ManagerComponent>("ManagerSystem").kind(flecs::PreFrame).without<_Components::Disabled>().each(ManagerSystem);
}
