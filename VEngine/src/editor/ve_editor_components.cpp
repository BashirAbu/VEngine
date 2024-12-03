#include "editor/ve_editor.h"
#include "ve_engine.h"
#include "components/ve_components.h"
#include "editor/ve_editor_elements.h"
#include "scenes/ve_scene.h"
#include "platform/ve_platform.h"
namespace VE 
{
	////Edtior stuff.
	//void Editor::DrawComponentElements(std::string name, flecs::entity entity) 
	//{
	//	ImGui::PushID(name.c_str());
	//	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	//	if (name == "TransformComponent")
	//	{
	//		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
	//		if (open)
	//		{
	//			Components::TransformComponent* tc = entity.get_mut<Components::TransformComponent>();
	//			EditorElement::Vec3(tc->localPosition, "Position");

	//			EditorElement::Vec3(tc->localRotation, "Rotation");

	//			EditorElement::Vec3(tc->localScale, "Scale");
	//			ImGui::TreePop();
	//		}
	//		
	//	}
	//	else if (name == "Camera2DComponent")
	//	{
	//		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
	//		if (ImGui::BeginPopupContextItem(0, 1))
	//		{
	//			if (ImGui::MenuItem("Remove"))
	//			{
	//				entity.remove<VE::Components::Camera2DComponent>(); 
	//				open = false;
	//			}
	//			ImGui::EndPopup();
	//		}

	//		if (open)
	//		{
	//			Components::Camera2DComponent* cc = entity.get_mut<Components::Camera2DComponent>();
	//			VE::EditorElement::Checkbox(cc->isMain, "Is Main");
	//			if (cc->isMain)
	//			{
	//				engine->sceneManager->currentScene->SetMainCamera(entity);
	//			}
	//			EditorElement::Float(cc->zoom, "Zoom");

	//			EditorElement::Vec2(cc->renderTargetSize, "Render Target Size");
	//			if (glm::vec2(cc->renderTarget.texture.width, cc->renderTarget.texture.height) != cc->renderTargetSize)
	//			{
	//				UnloadRenderTexture(cc->renderTarget);
	//				cc->renderTarget = LoadRenderTexture((int)cc->renderTargetSize.x, (int)cc->renderTargetSize.y, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	//				SetTextureFilter(cc->renderTarget.texture, TEXTURE_FILTER_BILINEAR);
	//			}

	//			EditorElement::Color(cc->backgroundColor, "Backgournd Color");

	//			ImGui::TreePop();
	//		}
	//	}
	//	else if (name == "SpriteComponent")
	//	{
	//		
	//		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
	//		if (ImGui::BeginPopupContextItem(0, 1))
	//		{
	//			if (ImGui::MenuItem("Remove"))
	//			{
	//				entity.remove<VE::Components::SpriteComponent>();
	//				open = false;
	//			}
	//			ImGui::EndPopup();
	//		}
	//		if (open)
	//		{
	//			Components::SpriteComponent* sp = entity.get_mut<Components::SpriteComponent>();

	//			EditorElement::FileSystem(sp->texturePath, "Texture");

	//			if (!sp->texturePath.empty())
	//			{
	//				sp->texture = AssetsManager::GetSingleton()->LoadTexture(sp->texturePath);
	//			}

	//			EditorElement::Vec2(sp->origin, "Origin");

	//			EditorElement::Int(sp->renderOrder, "Render Order");

	//			EditorElement::Color(sp->tintColor, "Tint Color");
	//			ImGui::TreePop();
	//		}
	//	}
	//	ImGui::PopID();
	//}
	
}