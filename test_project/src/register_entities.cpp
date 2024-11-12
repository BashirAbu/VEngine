
#include "ve_defines.h"
#include "manager_entity.h"
#include "player_entity.h"
VE_REGISTER_ENTITY_BEGIN()
VE_REGISTER_ENTITY(ManagerEntity)
VE_REGISTER_ENTITY(PlayerEntity)
VE_REGISTER_ENTITY_END()
#include "ve_engine.h"



extern "C" VE_PROJECT_API void OnSharedLibraryEntry()
{
    VE::Engine::GetSingleton()->RegisterEntity(VE_STRINGIFY(ManagerEntity));
    VE::Engine::GetSingleton()->RegisterEntity(VE_STRINGIFY(PlayerEntity));
}




