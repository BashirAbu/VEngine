#pragma once
#include <assert.h>
#include <stdint.h>
#ifdef _WIN32
#define UNICODE
#define VE_WIN32
#endif

//TODO: VE_DEBUG macros for other operating systems.
#ifdef VE_WIN32
#ifdef _DEBUG
#define VE_DEBUG
#endif
#endif

#define VE_OPENGL



#ifdef VE_DEBUG
#define VE_ASSERT(expression) assert(expression)
#define VE_STATIC_ASSERT(expression) _STATIC_ASSERT(expression)
#else
#define VE_ASSERT(expression)
#define VE_STATIC_ASSERT(expression)
#endif

#define VE_IMGUI

#define VE_EDITOR

#ifdef VE_EDITOR

#endif

#define VE_STRINGIFY(x) #x

#ifdef VE_EXPORT
#define VE_API __declspec(dllexport)
#else
#define VE_API __declspec(dllimport)
#endif

#ifdef VE_PROJECT_EXPORT
#define VE_PROJECT_API __declspec(dllexport)
#else
#define VE_PROJECT_API __declspec(dllimport)
#endif


#define VE_REGISTER_ENTITY_BEGIN()\
extern "C" VE_PROJECT_API VE::Entity* CreateProjectEntity(std::string entityName)\
{\
    if (0)\
    {}

#define VE_REGISTER_ENTITY(entity)\
    else if(#entity == entityName)\
    {\
        VE::Entity* ent = new entity(#entity);\
        ent->SetInternalName(entityName);\
        return ent;\
    }


#define VE_REGISTER_ENTITY_END() return nullptr;}


#ifdef VE_WIN32
#define CHANGE_CURRENT_WORKING_DIRECTORY _chdir
#else
#define CHANGE_CURRENT_WORKING_DIRECTORY chdir
#endif

#define VE_PROJECT_FILE_EXTENSION "VEProject"
#define VE_SCENE_FILE_EXTENSION "VEScene"
