#ifdef VE_EDITOR
#include "ve_defines.h"
#include "scenes/ve_scene.h"
#include "editor/ve_editor.h"
#include <imgui.h>
#include "editor/ve_editor_elements.h"
#include "components/ve_components.h"

#include "components/ve_components.h"

using namespace VE;
using namespace VE::Components;
using namespace VE::_Components;
using namespace VE::Components::UI;


void DrawTextureMapEditorUI(std::string name, flecs::entity entity, VE::_Components::TextureMap* TextureMap_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(408)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::_Components::TextureMap>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Color(TextureMap_->color, "Color");


			EditorElement::Float(TextureMap_->value, "Value");


			EditorElement::FileSystem(TextureMap_->texturePath, "Texture Path");


			EditorElement::Image(&TextureMap_->texture, "Texture");


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawVEMaterialEditorUI(std::string name, flecs::entity entity, VE::_Components::VEMaterial* VEMaterial_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(623)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::_Components::VEMaterial>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			VE::_Components::TextureMap* albedoMap_ = &VEMaterial_->albedoMap;
			DrawTextureMapEditorUI("albedoMap", entity, albedoMap_, false); 


			VE::_Components::TextureMap* specularMap_ = &VEMaterial_->specularMap;
			DrawTextureMapEditorUI("specularMap", entity, specularMap_, false); 


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawTransformComponentEditorUI(std::string name, flecs::entity entity, VE::Components::TransformComponent* TransformComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(436)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::TransformComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Vec3(TransformComponent_->localPosition, "Local Position");


			EditorElement::Vec3(TransformComponent_->localRotation, "Local Rotation");


			EditorElement::Vec3(TransformComponent_->localScale, "Local Scale");


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawSpriteComponentEditorUI(std::string name, flecs::entity entity, VE::Components::SpriteComponent* SpriteComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(277)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::SpriteComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::FileSystem(SpriteComponent_->texturePath, "Texture Path", SpriteComponent_, VE::Components::SpriteComponentTextureOnChange);


			EditorElement::Vec2(SpriteComponent_->origin, "Origin");


			EditorElement::Color(SpriteComponent_->tintColor, "Tint Color");


			EditorElement::Int(SpriteComponent_->renderOrder, "Render Order");


			EditorElement::FileSystem(SpriteComponent_->shaderPath, "Shader Path", SpriteComponent_, VE::Components::SpriteComponentShaderOnChange);


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawCamera2DComponentEditorUI(std::string name, flecs::entity entity, VE::Components::Camera2DComponent* Camera2DComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(391)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::Camera2DComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Vec2(Camera2DComponent_->renderTargetSize, "Render Target Size", Camera2DComponent_, VE::Components::Camera2DComponentRenderTargetSizeOnChange);


			EditorElement::Color(Camera2DComponent_->backgroundColor, "Background Color");


			EditorElement::Float(Camera2DComponent_->zoom, "Zoom");


			EditorElement::Checkbox(Camera2DComponent_->isMain, "Is Main", Camera2DComponent_, VE::Components::Camera2DComponentRenderIsMainOnChange);


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawCamera3DComponentEditorUI(std::string name, flecs::entity entity, VE::Components::Camera3DComponent* Camera3DComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(635)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::Camera3DComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Vec2(Camera3DComponent_->renderTargetSize, "Render Target Size", Camera3DComponent_, VE::Components::Camera3DComponentRenderTargetSizeOnChange);


			EditorElement::Color(Camera3DComponent_->backgroundColor, "Background Color");


			EditorElement::Float(Camera3DComponent_->zoom, "Zoom");


			EditorElement::Checkbox(Camera3DComponent_->isMain, "Is Main", Camera3DComponent_, VE::Components::Camera3DComponentRenderIsMainOnChange);


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawLightComponentEditorUI(std::string name, flecs::entity entity, VE::Components::LightComponent* LightComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(941)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::LightComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Color(LightComponent_->color, "Color");


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawModel3DComponentEditorUI(std::string name, flecs::entity entity, VE::Components::Model3DComponent* Model3DComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(244)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::Model3DComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			const char* items[] = { 
					"None",
					"Cube",
					"Sphere",
			};
			EditorElement::Combo(items, (int*)&(Model3DComponent_->basicMesh), 3, "Basic Mesh", Model3DComponent_, VE::Components::BasicModelOnChange);


			EditorElement::FileSystem(Model3DComponent_->modelFilepath, "Model Filepath", Model3DComponent_, VE::Components::ModelOnChange);


			{ImGui::PushID(((std::string)"materials" + (const char*)(int[1])(242)).c_str()); 
			ImGuiTreeNodeFlags _flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
			bool _open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), _flags, "materials");
			ImGui::SameLine();
			ImVec2 plusSize = ImGui::CalcTextSize("+");
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
			if(ImGui::Button("+", {plusSize.x * 4.0f, plusSize.y * 0.85f})){Model3DComponent_->materials.push_back({});}
			ImGui::PopStyleVar(2);
			int elementIndexToRemove = -1;
			if(_open){

				for(size_t i = 0; i < Model3DComponent_->materials.size(); i++){
						ImGui::PushID(((std::string)"materials" + (const char*)(size_t[1])i).c_str());
						ImGuiTreeNodeFlags __flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
						bool __open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), __flags, ((std::string)"materials[" + std::to_string(i) + "]").c_str());
						ImGui::SameLine();
					ImVec2 plusSize = ImGui::CalcTextSize("-");
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
					if(ImGui::Button("-", {plusSize.x * 4.0f, plusSize.y})){elementIndexToRemove = (int)i;}
					ImGui::PopStyleVar(2);
						if(__open){
			VE::_Components::VEMaterial* materials_ = &Model3DComponent_->materials[i];
			DrawVEMaterialEditorUI("materials", entity, materials_, false); 


						ImGui::TreePop();}
						ImGui::PopID();
					}
			if(elementIndexToRemove > 0 && elementIndexToRemove < Model3DComponent_->materials.size()) Model3DComponent_->materials.erase(Model3DComponent_->materials.begin() + elementIndexToRemove);
			ImGui::TreePop();
}
			ImGui::PopID();}
			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawUICanvasComponentEditorUI(std::string name, flecs::entity entity, VE::Components::UI::UICanvasComponent* UICanvasComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(653)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UICanvasComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Vec2(UICanvasComponent_->canvasSize, "Canvas Size");


			EditorElement::Checkbox(UICanvasComponent_->isMain, "Is Main");


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawUILabelComponentEditorUI(std::string name, flecs::entity entity, VE::Components::UI::UILabelComponent* UILabelComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(198)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UILabelComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::FileSystem(UILabelComponent_->fontFilepath, "Font Filepath", UILabelComponent_, VE::Components::UI::UILabelFontOnChange);


			EditorElement::String(UILabelComponent_->text, "Text", UILabelComponent_, VE::Components::UI::UILabelTextOnChange);


			EditorElement::Vec2(UILabelComponent_->origin, "Origin");


			EditorElement::Int(UILabelComponent_->renderOrder, "Render Order");


			EditorElement::Color(UILabelComponent_->color, "Color");


			EditorElement::Float(UILabelComponent_->size, "Size", UILabelComponent_, VE::Components::UI::UILabelSizeOnChange);


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawUIImageComponentEditorUI(std::string name, flecs::entity entity, VE::Components::UI::UIImageComponent* UIImageComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(869)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UIImageComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::FileSystem(UIImageComponent_->imageFilepath, "Image Filepath", UIImageComponent_, VE::Components::UI::UIImageComponentTextureOnChange);


			EditorElement::Vec2(UIImageComponent_->origin, "Origin");


			EditorElement::Color(UIImageComponent_->tintColor, "Tint Color");


			EditorElement::Int(UIImageComponent_->renderOrder, "Render Order");


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void DrawUIButtonComponentEditorUI(std::string name, flecs::entity entity, VE::Components::UI::UIButtonComponent* UIButtonComponent_, bool removable)
{
	ImGui::PushID((name + (const char*)(int[1])(806)).c_str()); 
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
		bool open = ImGui::TreeNodeEx((void*)((uint64_t)(entity)), flags, name.c_str());
		if(removable)
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if(ImGui::MenuItem("Remove"))
			{
				entity.remove<VE::Components::UI::UIButtonComponent>();
			}
			ImGui::EndPopup();
		}
		if (open)
		{
			
			EditorElement::Color(UIButtonComponent_->pressTintColor, "Press Tint Color");


			ImGui::TreePop();
		}
		ImGui::PopID();
}
void VE::Editor::DrawComponentElements(std::string name, flecs::entity entity)
{
	if(name == "TextureMap"){
			VE::_Components::TextureMap* TextureMap_ = entity.get_mut<VE::_Components::TextureMap>();
		DrawTextureMapEditorUI(name, entity,TextureMap_, true);
	}
	if(name == "VEMaterial"){
			VE::_Components::VEMaterial* VEMaterial_ = entity.get_mut<VE::_Components::VEMaterial>();
		DrawVEMaterialEditorUI(name, entity,VEMaterial_, true);
	}
	if(name == "TransformComponent"){
			VE::Components::TransformComponent* TransformComponent_ = entity.get_mut<VE::Components::TransformComponent>();
		DrawTransformComponentEditorUI(name, entity,TransformComponent_, false);
	}
	if(name == "SpriteComponent"){
			VE::Components::SpriteComponent* SpriteComponent_ = entity.get_mut<VE::Components::SpriteComponent>();
		DrawSpriteComponentEditorUI(name, entity,SpriteComponent_, true);
	}
	if(name == "Camera2DComponent"){
			VE::Components::Camera2DComponent* Camera2DComponent_ = entity.get_mut<VE::Components::Camera2DComponent>();
		DrawCamera2DComponentEditorUI(name, entity,Camera2DComponent_, true);
	}
	if(name == "Camera3DComponent"){
			VE::Components::Camera3DComponent* Camera3DComponent_ = entity.get_mut<VE::Components::Camera3DComponent>();
		DrawCamera3DComponentEditorUI(name, entity,Camera3DComponent_, true);
	}
	if(name == "LightComponent"){
			VE::Components::LightComponent* LightComponent_ = entity.get_mut<VE::Components::LightComponent>();
		DrawLightComponentEditorUI(name, entity,LightComponent_, true);
	}
	if(name == "Model3DComponent"){
			VE::Components::Model3DComponent* Model3DComponent_ = entity.get_mut<VE::Components::Model3DComponent>();
		DrawModel3DComponentEditorUI(name, entity,Model3DComponent_, true);
	}
	if(name == "UICanvasComponent"){
			VE::Components::UI::UICanvasComponent* UICanvasComponent_ = entity.get_mut<VE::Components::UI::UICanvasComponent>();
		DrawUICanvasComponentEditorUI(name, entity,UICanvasComponent_, true);
	}
	if(name == "UILabelComponent"){
			VE::Components::UI::UILabelComponent* UILabelComponent_ = entity.get_mut<VE::Components::UI::UILabelComponent>();
		DrawUILabelComponentEditorUI(name, entity,UILabelComponent_, true);
	}
	if(name == "UIImageComponent"){
			VE::Components::UI::UIImageComponent* UIImageComponent_ = entity.get_mut<VE::Components::UI::UIImageComponent>();
		DrawUIImageComponentEditorUI(name, entity,UIImageComponent_, true);
	}
	if(name == "UIButtonComponent"){
			VE::Components::UI::UIButtonComponent* UIButtonComponent_ = entity.get_mut<VE::Components::UI::UIButtonComponent>();
		DrawUIButtonComponentEditorUI(name, entity,UIButtonComponent_, true);
	}

}
#endif
