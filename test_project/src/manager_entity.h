#pragma once 
#include "ve_defines.h"
#include "entities/ve_entity.h"

class VE_PROJECT_API ManagerEntity : public VE::Entity
{
public:
	ManagerEntity(std::string name);
	~ManagerEntity();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
};