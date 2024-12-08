#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <filesystem>
#include <fstream>
#include <flecs.h>
#include <unordered_map>
#include <flecs/addons/meta.h>
#include <flecs/addons/json.h>
#include <nlohmann/json.hpp>
#define GLM_FORCE_SSE2
#define GLM_FORCE_AVX
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <assert.h>
#include <stdint.h>
#ifdef _WIN32
#define UNICODE
#define VE_WIN32
#endif


namespace VE 
{
	using NormalizedColor = glm::vec4;
}


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
#ifdef VE_WIN32
#ifndef VE_STATIC
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
#else

#define VE_API
#define VE_PROJECT_API

#endif
#endif

#ifndef VE_WIN32
#define VE_API
#define VE_PROJECT_API
#endif // !VE_WIN32



//Header tool macros.
#define VE_CLASS()
#define VE_PROPERTY()
#define VE_ENUM()
#define VE_FUNCTION()


#define VE_PROJECT_FILE_EXTENSION "VEProject"
#define VE_SCENE_FILE_EXTENSION "VEScene"
#define VE_CONSTRUCT_FILE_EXTENSION "VEConstruct"
