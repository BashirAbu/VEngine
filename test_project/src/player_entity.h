#pragma once 
#include "ve_defines.h"
#include "entities/ve_entity.h"

class VE_PROJECT_API PlayerEntity : public VE::Entity
{
public:
	PlayerEntity(std::string name);
	~PlayerEntity();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
};