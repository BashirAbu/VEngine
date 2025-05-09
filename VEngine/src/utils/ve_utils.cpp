﻿#include "ve_utils.h"
#include "components/ve_components.h"
#include "ve_font.h"
#include <codecvt>
#include <locale>
#include "ShapingEngine.hpp"
#include "utf8.h"
namespace VE
{

	Color GLMVec4ToRayColor(glm::vec4 color)
	{
		return Color{ uint8_t(color.r * 255.0f), uint8_t(color.g * 255.0f), uint8_t(color.b * 255.0f), uint8_t(color.a * 255.0f) };
	}


	Matrix GlmMat4ToRaylibMatrix(const glm::mat4& glmMatrix) {
		Matrix raylibMatrix = {
			glmMatrix[0][0], glmMatrix[1][0], glmMatrix[2][0], glmMatrix[3][0], // First column
			glmMatrix[0][1], glmMatrix[1][1], glmMatrix[2][1], glmMatrix[3][1], // Second column
			glmMatrix[0][2], glmMatrix[1][2], glmMatrix[2][2], glmMatrix[3][2], // Third column
			glmMatrix[0][3], glmMatrix[1][3], glmMatrix[2][3], glmMatrix[3][3]  // Fourth column
		};
		return raylibMatrix;
	}

	VE_API glm::mat4 RaylibMatrixToGlmMat4(const Matrix& raylibMatrix)
	{
		return glm::mat4(
			raylibMatrix.m0, raylibMatrix.m4, raylibMatrix.m8, raylibMatrix.m12,  // First column
			raylibMatrix.m1, raylibMatrix.m5, raylibMatrix.m9, raylibMatrix.m13,  // Second column
			raylibMatrix.m2, raylibMatrix.m6, raylibMatrix.m10, raylibMatrix.m14, // Third column
			raylibMatrix.m3, raylibMatrix.m7, raylibMatrix.m11, raylibMatrix.m15  // Fourth column
		);
	}


	std::filesystem::path GetRelativePath(std::filesystem::path path)
	{
		return path.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().generic_string() + "/assets");
	}
	std::filesystem::path GetFullPath(std::filesystem::path path)
	{
		return Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().string() + "/assets/" + path.string();
	}
	VE_API void RaylibDrawTexturTargeteLetterBox(RenderTexture renderTarget, glm::vec2 screenSize)
	{
		float scale = glm::min(screenSize.x / renderTarget.texture.width, screenSize.y / renderTarget.texture.height);


		DrawTexturePro(renderTarget.texture, { 0.0f, 0.0f, (float)renderTarget.texture.width, (float)-renderTarget.texture.height },
			{ (screenSize.x - (renderTarget.texture.width * scale)) * .5f, (screenSize.y - (renderTarget.texture.height * scale)) * .5f, renderTarget.texture.width * scale, renderTarget.texture.height * scale }
		, { 0.0f, 0.0f }, 0.0f, WHITE);
	}
	VE_API Texture RaylibGetTextureFromText_UTF8(Font* font, std::string text, float spacing)
	{
		Texture result = {};

		std::wstring wideString;
		utf8::utf8to16(text.begin(), text.end(), std::back_inserter(wideString));
		//std::string processedString = ShapingEngine::render(wideString, true, true);
		//wideString = L"";
		//utf8::utf8to16(processedString.begin(), processedString.end(), std::back_inserter(wideString));



		std::vector<Glyph> glyphs;
		int32_t imageWidth = 0;
		int32_t imageHeight = 0;
		int32_t baseline = 0;
		//bearing, advance.
		for (wchar_t c : wideString)
		{
			Glyph g = font->GetGlypth(c);
			glyphs.push_back(g);
			imageHeight = glm::max(imageHeight, g.height);
			baseline = glm::max(baseline, g.bearingY);
			imageWidth += (g.bearingX > 0 ? g.bearingX : 0) + g.advance;
		}

		imageHeight += baseline;


		uint32_t* imgBuffer = (uint32_t*)malloc(imageWidth * imageHeight * sizeof(uint32_t));
		memset(imgBuffer, 0, imageWidth * imageHeight * sizeof(uint32_t));
		int32_t xOffset = 0;
		for (int32_t i = 0; i < glyphs.size(); i++)
		{

			for (int32_t j = 0; j < glyphs[i].height; j++)
			{
				for (int32_t k = 0; k < glyphs[i].width; k++)
				{
					uint32_t* gPixel = (uint32_t*)glyphs[i].img.data;
					int32_t bufferY = baseline - glyphs[i].bearingY + j;
					int32_t _offset = (k + xOffset + glyphs[i].bearingX);
					if (_offset > 0)
					{
						int32_t index = bufferY * imageWidth + _offset;
						imgBuffer[index] = glm::max(gPixel[j * glyphs[i].width + k], imgBuffer[index]);
					}
				}
			}
			xOffset += glyphs[i].advance;
		}

		Image textImage = {};
		textImage.data = imgBuffer;
		textImage.width = imageWidth;
		textImage.height = imageHeight;
		textImage.mipmaps = 1;
		textImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

		result = LoadTextureFromImage(textImage);
		free(imgBuffer);
		SetTextureFilter(result, TEXTURE_FILTER_BILINEAR);
		SetTextureWrap(result, TEXTURE_WRAP_CLAMP);

		return result;
	}
}
