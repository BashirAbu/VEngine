#include "ve_defines.h"

void ProjectGeneratedRegistration();

extern "C" VE_PROJECT_API void OnSharedLibraryEntry(flecs::world& world) 
{
	ProjectGeneratedRegistration();
}