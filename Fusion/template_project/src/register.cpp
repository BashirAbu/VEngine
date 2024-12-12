#include "ve_defines.h"
#include <raylib.h>
#include "components.h"
#include "systems.h"
#include "components/ve_components.h"

void ProjectGeneratedRegistration();

extern "C" VE_PROJECT_API void OnSharedLibraryEntry(flecs::world& world) 
{
	ProjectGeneratedRegistration();
}