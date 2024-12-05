#pragma once
#include "ve_defines.h"
#include <raylib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
namespace VE 
{

	struct Glyph 
	{
		Texture2D texture;
		int32_t width;
		int32_t height;
		int32_t bearingX;
		int32_t bearingY;
		int32_t advance;
	};

	class VE_API Font 
	{
	public:
		Font() : fontSize(48){}
		Font(std::filesystem::path filepath, int32_t fontSize);
		~Font();

		bool LoadGlyph(FT_UInt character);
		Glyph GetGlypth(FT_UInt character);
		int32_t GetFontSize() { return fontSize; }
		FT_Face GetFreeTypeFace() { return face; }
	private:

		int32_t fontSize;

		FT_Face face;

		std::unordered_map<FT_UInt, Glyph> glyphs;

		static size_t fontsCount;
	};
}