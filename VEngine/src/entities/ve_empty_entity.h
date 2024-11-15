#pragma once
#include "ve_defines.h"
#include "ve_entity.h"

namespace VE 
{
	class VE_API EmptyEntity : public Entity 
	{
	public:
		EmptyEntity(std::string name = "");
		~EmptyEntity();

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Serialize(nlohmann::json& json) override;
		virtual void Deserialize(nlohmann::json& json) override;
		virtual void DrawEditorUI() override;
	private:
	};
}