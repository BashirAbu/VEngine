#include "ve_utils.h"
#include "components/ve_components.h"
#include "ve_font.h"
#include <codecvt>
#include <locale>
#include "ShapingEngine.hpp"
namespace VE 
{

    Color GLMVec4ToRayColor(glm::vec4 color)
    {
        return Color{ uint8_t(color.r * 255.0f), uint8_t(color.g * 255.0f), uint8_t(color.b * 255.0f), uint8_t(color.a * 255.0f) };
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


        DrawTexturePro(renderTarget.texture, {0.0f, 0.0f, (float) renderTarget.texture.width, (float) -renderTarget.texture.height},
            {(screenSize.x - (renderTarget.texture.width * scale)) * .5f, (screenSize.y - (renderTarget.texture.height * scale)) * .5f, renderTarget.texture.width * scale, renderTarget.texture.height * scale }
        , {0.0f, 0.0f}, 0.0f, WHITE);
    }
    VE_API Texture RaylibGetTextureFromText_UTF8(Font* font, std::string text, float spacing)
    {
        Texture result = {};

		

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

		std::wstring wideString = converter.from_bytes(text);

		std::string processedString = ShapingEngine::render(wideString);

		wideString = converter.from_bytes(processedString);

		

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
			imageWidth += (g.bearingX > 0? g.bearingX : 0) + g.advance;
		}

		imageHeight += baseline;

		
		std::vector<uint32_t> imgBuffer(imageWidth * imageHeight);
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
		textImage.data = imgBuffer.data();
		textImage.width = imageWidth;
		textImage.height = imageHeight;
		textImage.mipmaps = 1;
		textImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
		result = LoadTextureFromImage(textImage);
		SetTextureFilter(result, TEXTURE_FILTER_BILINEAR);
		SetTextureWrap(result, TEXTURE_WRAP_CLAMP);

        return result;
    }
}
