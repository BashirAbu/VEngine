#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "components/ve_components.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::Components::UI;
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


void Serialize_UICanvasComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "UICanvasComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::UI::UICanvasComponent>* comp = (flecs::component<VE::Components::UI::UICanvasComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<glm::vec2>("canvasSize").member<bool>("isMain"))
		.serialize([](const flecs::serializer* s, const VE::Components::UI::UICanvasComponent* data) -> int		{
		s->member("canvasSize");
		s->value(data->canvasSize);
		s->member("isMain");
		s->value(data->isMain);
			 return 0;
		}).ensure_member([](VE::Components::UI::UICanvasComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "canvasSize")) { return &data->canvasSize;}
			else if (!strcmp(member, "isMain")) { return &data->isMain;}
		return nullptr;
		});
	}
}


void Serialize_UILabelComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "UILabelComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::UI::UILabelComponent>* comp = (flecs::component<VE::Components::UI::UILabelComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<std::filesystem::path>("fontFilepath").member<std::string>("text").member<glm::vec2>("origin").member<int32_t>("renderOrder").member<NormalizedColor>("color").member<float>("size"))
		.serialize([](const flecs::serializer* s, const VE::Components::UI::UILabelComponent* data) -> int		{
		s->member("fontFilepath");
		s->value(data->fontFilepath);
		s->member("text");
		s->value(data->text);
		s->member("origin");
		s->value(data->origin);
		s->member("renderOrder");
		s->value(data->renderOrder);
		s->member("color");
		s->value(data->color);
		s->member("size");
		s->value(data->size);
			 return 0;
		}).ensure_member([](VE::Components::UI::UILabelComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "fontFilepath")) { return &data->fontFilepath;}
			else if (!strcmp(member, "text")) { return &data->text;}
			else if (!strcmp(member, "origin")) { return &data->origin;}
			else if (!strcmp(member, "renderOrder")) { return &data->renderOrder;}
			else if (!strcmp(member, "color")) { return &data->color;}
			else if (!strcmp(member, "size")) { return &data->size;}
		return nullptr;
		});
	}
}


void Serialize_UIImageComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "UIImageComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::UI::UIImageComponent>* comp = (flecs::component<VE::Components::UI::UIImageComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<std::filesystem::path>("imageFilepath").member<glm::vec2>("origin").member<NormalizedColor>("tintColor").member<int32_t>("renderOrder"))
		.serialize([](const flecs::serializer* s, const VE::Components::UI::UIImageComponent* data) -> int		{
		s->member("imageFilepath");
		s->value(data->imageFilepath);
		s->member("origin");
		s->value(data->origin);
		s->member("tintColor");
		s->value(data->tintColor);
		s->member("renderOrder");
		s->value(data->renderOrder);
			 return 0;
		}).ensure_member([](VE::Components::UI::UIImageComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "imageFilepath")) { return &data->imageFilepath;}
			else if (!strcmp(member, "origin")) { return &data->origin;}
			else if (!strcmp(member, "tintColor")) { return &data->tintColor;}
			else if (!strcmp(member, "renderOrder")) { return &data->renderOrder;}
		return nullptr;
		});
	}
}


void Serialize_UIButtonComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "UIButtonComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::UI::UIButtonComponent>* comp = (flecs::component<VE::Components::UI::UIButtonComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<std::filesystem::path>("fontFilepath").member<std::string>("text").member<glm::vec2>("textOrigin").member<NormalizedColor>("textColor").member<float>("textSize").member<std::filesystem::path>("imageFilepath").member<glm::vec2>("imageOrigin").member<NormalizedColor>("tintColor").member<NormalizedColor>("pressTintColor").member<int32_t>("imageRenderOrder"))
		.serialize([](const flecs::serializer* s, const VE::Components::UI::UIButtonComponent* data) -> int		{
		s->member("fontFilepath");
		s->value(data->fontFilepath);
		s->member("text");
		s->value(data->text);
		s->member("textOrigin");
		s->value(data->textOrigin);
		s->member("textColor");
		s->value(data->textColor);
		s->member("textSize");
		s->value(data->textSize);
		s->member("imageFilepath");
		s->value(data->imageFilepath);
		s->member("imageOrigin");
		s->value(data->imageOrigin);
		s->member("tintColor");
		s->value(data->tintColor);
		s->member("pressTintColor");
		s->value(data->pressTintColor);
		s->member("imageRenderOrder");
		s->value(data->imageRenderOrder);
			 return 0;
		}).ensure_member([](VE::Components::UI::UIButtonComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "fontFilepath")) { return &data->fontFilepath;}
			else if (!strcmp(member, "text")) { return &data->text;}
			else if (!strcmp(member, "textOrigin")) { return &data->textOrigin;}
			else if (!strcmp(member, "textColor")) { return &data->textColor;}
			else if (!strcmp(member, "textSize")) { return &data->textSize;}
			else if (!strcmp(member, "imageFilepath")) { return &data->imageFilepath;}
			else if (!strcmp(member, "imageOrigin")) { return &data->imageOrigin;}
			else if (!strcmp(member, "tintColor")) { return &data->tintColor;}
			else if (!strcmp(member, "pressTintColor")) { return &data->pressTintColor;}
			else if (!strcmp(member, "imageRenderOrder")) { return &data->imageRenderOrder;}
		return nullptr;
		});
	}
}


void EngineGeneratedSerialization()
{
	 Serialize_TransformComponent();
	 Serialize_SpriteComponent();
	 Serialize_Camera2DComponent();
	 Serialize_UICanvasComponent();
	 Serialize_UILabelComponent();
	 Serialize_UIImageComponent();
	 Serialize_UIButtonComponent();
}




void EngineGeneratedRegistration()
{
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::TransformComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::SpriteComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::Camera2DComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UICanvasComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UILabelComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UIImageComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UIButtonComponent>();
	EngineGeneratedSerialization();
}
