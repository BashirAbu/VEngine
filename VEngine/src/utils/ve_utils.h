#pragma once
#include "ve_defines.h"
#include "ve_renderer.h"
#include <raylib.h>
namespace VE 
{
	VE_API Color GLMVec4ToRayColor(glm::vec4 color);

	VE_API std::filesystem::path GetRelativePath(std::filesystem::path path);
	VE_API std::filesystem::path GetFullPath(std::filesystem::path path);

	VE_API void RaylibDrawTexturTargeteLetterBox(RenderTexture renderTarget, glm::vec2 screenSize);
	VE_API Texture RaylibGetTextureFromText_UTF8(class Font* font, std::string text);
}