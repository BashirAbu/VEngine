#include "editor/ve_editor.h"
#include "ve_engine.h"
#include "components/ve_components.h"
#include "editor/ve_editor_elements.h"
#include "scenes/ve_scene.h"
#include "platform/ve_platform.h"
namespace VE 
{
	//Serialization stuff
	void Scene::AddMetaData(flecs::entity comp)
	{
		std::string compName = (std::string)comp.name().c_str();
		if (compName.find("vec<2") != std::string::npos)
		{
			flecs::component<glm::vec2>* vec2 = (flecs::component<glm::vec2>*) & comp;
			vec2->member<float>("x").member<float>("y");
		}
		else if (compName.find("vec<3") != std::string::npos)
		{
			flecs::component<glm::vec3>* vec3 = (flecs::component<glm::vec3>*) & comp;
			vec3->member<float>("x").member<float>("y").member<float>("z");
		}
		else if (compName.find("vec<4") != std::string::npos)
		{
			flecs::component<glm::vec4>* vec4 = (flecs::component<glm::vec4>*) & comp;
			vec4->member<float>("x").member<float>("y").member<float>("z").member<float>("w");
		}
		else if (compName.find("TransformComponent") != std::string::npos)
		{
			flecs::component<Components::TransformComponent>* tc = (flecs::component<Components::TransformComponent>*)&comp;
			tc->member<glm::vec3>("localPosition").member<glm::vec3>("localRotation").member<glm::vec3>("localScale");
		}
		else if (compName.find("SpriteComponent") != std::string::npos)
		{
			flecs::component<Components::SpriteComponent>* sc = (flecs::component<Components::SpriteComponent>*) & comp;
			sc->member<std::filesystem::path>("texturePath").member<glm::vec2>("origin").member<glm::vec4>("tintColor"). member<flecs::i32_t>("renderOrder");
		}
		else if (compName.find("Camera2DComponent") != std::string::npos)
		{
			flecs::component<Components::Camera2DComponent>* cc = (flecs::component<Components::Camera2DComponent>*) & comp;
			cc->member<glm::vec2>("renderTargetSize").member<glm::vec4>("backgroundColor").member<float>("zoom").member<bool>("isMain");
		}
	}


	
	//Edtior stuff.
	void Editor::DrawComponentElements(std::string name, flecs::entity entity) 
	{
		ImGui::PushID(name.c_str());
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		if (name == "TransformComponent")
		{
			bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
			if (open)
			{
				Components::TransformComponent* tc = entity.get_mut<Components::TransformComponent>();
				EditorElement::Vec3(tc->localPosition, "Position");

				EditorElement::Vec3(tc->localRotation, "Rotation");

				EditorElement::Vec3(tc->localScale, "Scale");
				ImGui::TreePop();
			}
			
		}
		else if (name == "Camera2DComponent")
		{
			bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
			if (ImGui::BeginPopupContextItem(0, 1))
			{
				if (ImGui::MenuItem("Remove"))
				{
					entity.remove<VE::Components::Camera2DComponent>(); 
					open = false;
				}
				ImGui::EndPopup();
			}

			if (open)
			{
				Components::Camera2DComponent* cc = entity.get_mut<Components::Camera2DComponent>();
				VE::EditorElement::Checkbox(cc->isMain, "Is Main");
				if (cc->isMain)
				{
					engine->sceneManager->currentScene->SetMainCamera(entity);
				}
				EditorElement::Float(cc->camera.zoom, "Zoom");
				glm::vec2 renderTargetSize = glm::vec2(cc->renderTarget.texture.width, cc->renderTarget.texture.height);
				EditorElement::Vec2(renderTargetSize, "Render Target Size");
				if (glm::vec2(cc->renderTarget.texture.width, cc->renderTarget.texture.height) != renderTargetSize)
				{
					UnloadRenderTexture(cc->renderTarget);
					cc->renderTarget = LoadRenderTexture((int)renderTargetSize.x, (int)renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
					SetTextureFilter(cc->renderTarget.texture, TEXTURE_FILTER_BILINEAR);
				}

				EditorElement::Color(cc->backgroundColor, "Backgournd Color");

				ImGui::TreePop();
			}
		}
		else if (name == "SpriteComponent")
		{
			
			bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
			if (ImGui::BeginPopupContextItem(0, 1))
			{
				if (ImGui::MenuItem("Remove"))
				{
					entity.remove<VE::Components::SpriteComponent>();
					open = false;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				Components::SpriteComponent* sp = entity.get_mut<Components::SpriteComponent>();

				EditorElement::FileSystem(sp->texturePath, "Texture");

				if (!sp->texturePath.empty())
				{
					sp->texture = AssetsManager::GetSingleton()->LoadTexture(sp->texturePath);
				}

				EditorElement::Vec2(sp->origin, "Origin");

				EditorElement::Int(sp->renderOrder, "Render Order");

				EditorElement::Color(sp->tintColor, "Tint Color");
				ImGui::TreePop();
			}
		}
		ImGui::PopID();
	}
	
}