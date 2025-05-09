#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "components/ve_components.h"

#include "components/ve_components.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::Components::UI;
using namespace VE::_Components;


void Serialize_TextureMap()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "TextureMap"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::_Components::TextureMap>* comp = (flecs::component<VE::_Components::TextureMap>*)&compEntity; 
		comp->opaque(comp->world().component().member<NormalizedColor>("color").member<float>("value").member<std::filesystem::path>("texturePath").member<Texture>("texture"))
		.serialize([](const flecs::serializer* s, const VE::_Components::TextureMap* data) -> int		{
		s->member("color");
		s->value(data->color);
		s->member("value");
		s->value(data->value);
		s->member("texturePath");
		s->value(data->texturePath);
		s->member("texture");
		s->value(data->texture);
			 return 0;
		}).ensure_member([](VE::_Components::TextureMap* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "color")) { return &data->color;}
			else if (!strcmp(member, "value")) { return &data->value;}
			else if (!strcmp(member, "texturePath")) { return &data->texturePath;}
			else if (!strcmp(member, "texture")) { return &data->texture;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::_Components::TextureMap>>().opaque(VE::std_vector_support<VE::_Components::TextureMap>);
	}
}


void Serialize_VEMaterial()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "VEMaterial"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::_Components::VEMaterial>* comp = (flecs::component<VE::_Components::VEMaterial>*)&compEntity; 
		comp->opaque(comp->world().component().member<TextureMap>("albedoMap").member<TextureMap>("specularMap").member<TextureMap>("ambientOcclusionMap"))
		.serialize([](const flecs::serializer* s, const VE::_Components::VEMaterial* data) -> int		{
		s->member("albedoMap");
		s->value(data->albedoMap);
		s->member("specularMap");
		s->value(data->specularMap);
		s->member("ambientOcclusionMap");
		s->value(data->ambientOcclusionMap);
			 return 0;
		}).ensure_member([](VE::_Components::VEMaterial* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "albedoMap")) { return &data->albedoMap;}
			else if (!strcmp(member, "specularMap")) { return &data->specularMap;}
			else if (!strcmp(member, "ambientOcclusionMap")) { return &data->ambientOcclusionMap;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::_Components::VEMaterial>>().opaque(VE::std_vector_support<VE::_Components::VEMaterial>);
	}
}


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
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::TransformComponent>>().opaque(VE::std_vector_support<VE::Components::TransformComponent>);
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
		comp->opaque(comp->world().component().member<std::filesystem::path>("texturePath").member<glm::vec2>("origin").member<NormalizedColor>("tintColor").member<int32_t>("renderOrder").member<std::filesystem::path>("shaderPath"))
		.serialize([](const flecs::serializer* s, const VE::Components::SpriteComponent* data) -> int		{
		s->member("texturePath");
		s->value(data->texturePath);
		s->member("origin");
		s->value(data->origin);
		s->member("tintColor");
		s->value(data->tintColor);
		s->member("renderOrder");
		s->value(data->renderOrder);
		s->member("shaderPath");
		s->value(data->shaderPath);
			 return 0;
		}).ensure_member([](VE::Components::SpriteComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "texturePath")) { return &data->texturePath;}
			else if (!strcmp(member, "origin")) { return &data->origin;}
			else if (!strcmp(member, "tintColor")) { return &data->tintColor;}
			else if (!strcmp(member, "renderOrder")) { return &data->renderOrder;}
			else if (!strcmp(member, "shaderPath")) { return &data->shaderPath;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::SpriteComponent>>().opaque(VE::std_vector_support<VE::Components::SpriteComponent>);
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
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::Camera2DComponent>>().opaque(VE::std_vector_support<VE::Components::Camera2DComponent>);
	}
}


void Serialize_Camera3DComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "Camera3DComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::Camera3DComponent>* comp = (flecs::component<VE::Components::Camera3DComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<glm::vec2>("renderTargetSize").member<NormalizedColor>("backgroundColor").member<float>("zoom").member<bool>("isMain").member<std::filesystem::path>("skyboxTexturePath").member<Texture>("skyboxTexture"))
		.serialize([](const flecs::serializer* s, const VE::Components::Camera3DComponent* data) -> int		{
		s->member("renderTargetSize");
		s->value(data->renderTargetSize);
		s->member("backgroundColor");
		s->value(data->backgroundColor);
		s->member("zoom");
		s->value(data->zoom);
		s->member("isMain");
		s->value(data->isMain);
		s->member("skyboxTexturePath");
		s->value(data->skyboxTexturePath);
		s->member("skyboxTexture");
		s->value(data->skyboxTexture);
			 return 0;
		}).ensure_member([](VE::Components::Camera3DComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "renderTargetSize")) { return &data->renderTargetSize;}
			else if (!strcmp(member, "backgroundColor")) { return &data->backgroundColor;}
			else if (!strcmp(member, "zoom")) { return &data->zoom;}
			else if (!strcmp(member, "isMain")) { return &data->isMain;}
			else if (!strcmp(member, "skyboxTexturePath")) { return &data->skyboxTexturePath;}
			else if (!strcmp(member, "skyboxTexture")) { return &data->skyboxTexture;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::Camera3DComponent>>().opaque(VE::std_vector_support<VE::Components::Camera3DComponent>);
	}
}


void Serialize_DirectionalLightComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "DirectionalLightComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::DirectionalLightComponent>* comp = (flecs::component<VE::Components::DirectionalLightComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<NormalizedColor>("color").member<NormalizedColor>("ambient").member<NormalizedColor>("specular"))
		.serialize([](const flecs::serializer* s, const VE::Components::DirectionalLightComponent* data) -> int		{
		s->member("color");
		s->value(data->color);
		s->member("ambient");
		s->value(data->ambient);
		s->member("specular");
		s->value(data->specular);
			 return 0;
		}).ensure_member([](VE::Components::DirectionalLightComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "color")) { return &data->color;}
			else if (!strcmp(member, "ambient")) { return &data->ambient;}
			else if (!strcmp(member, "specular")) { return &data->specular;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::DirectionalLightComponent>>().opaque(VE::std_vector_support<VE::Components::DirectionalLightComponent>);
	}
}


void Serialize_PointLightComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "PointLightComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::PointLightComponent>* comp = (flecs::component<VE::Components::PointLightComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<NormalizedColor>("color").member<NormalizedColor>("ambient").member<NormalizedColor>("specular").member<float>("constant").member<float>("linear").member<float>("quadratic"))
		.serialize([](const flecs::serializer* s, const VE::Components::PointLightComponent* data) -> int		{
		s->member("color");
		s->value(data->color);
		s->member("ambient");
		s->value(data->ambient);
		s->member("specular");
		s->value(data->specular);
		s->member("constant");
		s->value(data->constant);
		s->member("linear");
		s->value(data->linear);
		s->member("quadratic");
		s->value(data->quadratic);
			 return 0;
		}).ensure_member([](VE::Components::PointLightComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "color")) { return &data->color;}
			else if (!strcmp(member, "ambient")) { return &data->ambient;}
			else if (!strcmp(member, "specular")) { return &data->specular;}
			else if (!strcmp(member, "constant")) { return &data->constant;}
			else if (!strcmp(member, "linear")) { return &data->linear;}
			else if (!strcmp(member, "quadratic")) { return &data->quadratic;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::PointLightComponent>>().opaque(VE::std_vector_support<VE::Components::PointLightComponent>);
	}
}


void Serialize_SpotLightComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "SpotLightComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::SpotLightComponent>* comp = (flecs::component<VE::Components::SpotLightComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<NormalizedColor>("color").member<NormalizedColor>("ambient").member<NormalizedColor>("specular").member<float>("cutOff").member<float>("outerCutOff"))
		.serialize([](const flecs::serializer* s, const VE::Components::SpotLightComponent* data) -> int		{
		s->member("color");
		s->value(data->color);
		s->member("ambient");
		s->value(data->ambient);
		s->member("specular");
		s->value(data->specular);
		s->member("cutOff");
		s->value(data->cutOff);
		s->member("outerCutOff");
		s->value(data->outerCutOff);
			 return 0;
		}).ensure_member([](VE::Components::SpotLightComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "color")) { return &data->color;}
			else if (!strcmp(member, "ambient")) { return &data->ambient;}
			else if (!strcmp(member, "specular")) { return &data->specular;}
			else if (!strcmp(member, "cutOff")) { return &data->cutOff;}
			else if (!strcmp(member, "outerCutOff")) { return &data->outerCutOff;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::SpotLightComponent>>().opaque(VE::std_vector_support<VE::Components::SpotLightComponent>);
	}
}


void Serialize_Model3DComponent()
{
	 flecs::entity compEntity = VE::Scene::GetSingleton()->GetFlecsWorld().query<flecs::Component>().find([](flecs::entity e, flecs::Component& c)	{
		if(!strcmp(e.name().c_str(), "Model3DComponent"))
			return e;
		return flecs::entity();	}); 
	if(compEntity)
	{
		flecs::component<VE::Components::Model3DComponent>* comp = (flecs::component<VE::Components::Model3DComponent>*)&compEntity; 
		comp->opaque(comp->world().component().member<_Components::BasicMesh>("basicMesh").member<std::filesystem::path>("modelFilepath").member<std::vector<_Components::VEMaterial>>("materials"))
		.serialize([](const flecs::serializer* s, const VE::Components::Model3DComponent* data) -> int		{
		s->member("basicMesh");
		s->value(data->basicMesh);
		s->member("modelFilepath");
		s->value(data->modelFilepath);
		s->member("materials");
		s->value(data->materials);
			 return 0;
		}).ensure_member([](VE::Components::Model3DComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "basicMesh")) { return &data->basicMesh;}
			else if (!strcmp(member, "modelFilepath")) { return &data->modelFilepath;}
			else if (!strcmp(member, "materials")) { return &data->materials;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::Model3DComponent>>().opaque(VE::std_vector_support<VE::Components::Model3DComponent>);
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
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::UI::UICanvasComponent>>().opaque(VE::std_vector_support<VE::Components::UI::UICanvasComponent>);
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
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::UI::UILabelComponent>>().opaque(VE::std_vector_support<VE::Components::UI::UILabelComponent>);
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
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::UI::UIImageComponent>>().opaque(VE::std_vector_support<VE::Components::UI::UIImageComponent>);
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
		comp->opaque(comp->world().component().member<NormalizedColor>("pressTintColor"))
		.serialize([](const flecs::serializer* s, const VE::Components::UI::UIButtonComponent* data) -> int		{
		s->member("pressTintColor");
		s->value(data->pressTintColor);
			 return 0;
		}).ensure_member([](VE::Components::UI::UIButtonComponent* data, const char* member) -> void*
		{
			if(0){ return nullptr;}
			else if (!strcmp(member, "pressTintColor")) { return &data->pressTintColor;}
		return nullptr;
		});
		flecs::world w = comp->world();
		w.component<std::vector<VE::Components::UI::UIButtonComponent>>().opaque(VE::std_vector_support<VE::Components::UI::UIButtonComponent>);
	}
}


void EngineGeneratedSerialization()
{
	 Serialize_TextureMap();
	 Serialize_VEMaterial();
	 Serialize_TransformComponent();
	 Serialize_SpriteComponent();
	 Serialize_Camera2DComponent();
	 Serialize_Camera3DComponent();
	 Serialize_DirectionalLightComponent();
	 Serialize_PointLightComponent();
	 Serialize_SpotLightComponent();
	 Serialize_Model3DComponent();
	 Serialize_UICanvasComponent();
	 Serialize_UILabelComponent();
	 Serialize_UIImageComponent();
	 Serialize_UIButtonComponent();
}




void EngineGeneratedRegistration()
{
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::_Components::TextureMap>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::_Components::VEMaterial>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::TransformComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::SpriteComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::Camera2DComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::Camera3DComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::DirectionalLightComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::PointLightComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::SpotLightComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::Model3DComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UICanvasComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UILabelComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UIImageComponent>();
	VE::Scene::GetSingleton()->GetFlecsWorld().component<VE::Components::UI::UIButtonComponent>();
	EngineGeneratedSerialization();
}
