#include "ve_defines.h"
#include "components.h"
void ProjectGeneratedRegistration();

extern "C" VE_PROJECT_API void OnSharedLibraryEntry(flecs::world& world)
{
	ProjectGeneratedRegistration();
}