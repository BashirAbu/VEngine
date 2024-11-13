#pragma once
#include "ve_defines.h"
#include "ve_engine.h"
class ProjectDialog
{
public:
	ProjectDialog();
	~ProjectDialog();
	bool Run(VE::ProjectDetails* projectDetails);
private:
	bool openProjectDetailsPopup = false;
};

