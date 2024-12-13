#pragma once
#include "ve_defines.h"
#include "ve_renderer.h"
#include <raylib.h>
namespace VE 
{
	VE_API Color GLMVec4ToRayColor(glm::vec4 color);

	VE_API Matrix GlmMat4ToRaylibMatrix(const glm::mat4& glmMatrix);
	VE_API glm::mat4 RaylibMatrixToGlmMat4(const Matrix& raylibMatrix);

	VE_API std::filesystem::path GetRelativePath(std::filesystem::path path);
	VE_API std::filesystem::path GetFullPath(std::filesystem::path path);

	VE_API void RaylibDrawTexturTargeteLetterBox(RenderTexture renderTarget, glm::vec2 screenSize);
	VE_API Texture RaylibGetTextureFromText_UTF8(class Font* font, std::string text, float spacing = 1.0f);
}