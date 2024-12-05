#include "ve_font.h"

namespace VE 
{
	FT_Library freetype = nullptr;
	size_t Font::fontsCount = 0;

	VE::Font::Font(std::filesystem::path filepath, int32_t fontSize) : fontSize(fontSize)
	{
		if (!freetype)
		{
			if (FT_Init_FreeType(&freetype)) 
			{
				TraceLog(LOG_ERROR, "Failed to initialize FreeType");
				return;
			}
		}

		FT_Error error = FT_New_Face(freetype, filepath.generic_string().c_str(), 0, &face);
		if (error)
		{
			TraceLog(LOG_ERROR, "Failed to open font file : %s", filepath.generic_string().c_str());
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, fontSize);

		fontsCount++;
	}

	Font::~Font()
	{
		if (fontsCount == 0)
		{
			FT_Done_FreeType(freetype);
		}

		FT_Done_Face(face);

		for (auto& glyph : glyphs)
		{
			UnloadTexture(glyph.second.texture);
		}
	}
	bool Font::LoadGlyph(FT_UInt character)
	{
		if (FT_Load_Char(face, character, FT_LOAD_RENDER))
		{
			TraceLog(LOG_ERROR, "Failed to load glyph");
			return false;
		}

		//rgba
		uint8_t* buffer = new uint8_t[face->glyph->bitmap.width * face->glyph->bitmap.rows * sizeof(uint8_t) * 4]{};
		
		struct Col 
		{
			uint8_t r, g, b, a;
		};

		Col* ptr = (Col*)buffer;

		for (size_t i = 0; i < face->glyph->bitmap.rows; i++)
		{
			for (size_t j = 0; j < face->glyph->bitmap.width; j++)
			{
				uint8_t glyphGrayLevel = face->glyph->bitmap.buffer[i * face->glyph->bitmap.width + j];
				ptr[i * face->glyph->bitmap.width + j] = {
					 glyphGrayLevel,
					 glyphGrayLevel,
					 glyphGrayLevel,
					 glyphGrayLevel
				};
			}
		}
		//LoadImageRaw
		Image img = {};
		img.data = buffer;
		img.width = face->glyph->bitmap.width;
		img.height = face->glyph->bitmap.rows;
		img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
		img.mipmaps = 1;

		Glyph glyph = {};

		glyph.texture = LoadTextureFromImage(img);
		SetTextureFilter(glyph.texture, TEXTURE_FILTER_BILINEAR);
		SetTextureWrap(glyph.texture, TEXTURE_WRAP_CLAMP);
		glyph.width = face->glyph->bitmap.width;
		glyph.height = face->glyph->bitmap.rows;
		glyph.bearingX = face->glyph->bitmap_left;
		glyph.bearingY = face->glyph->bitmap_top;
		glyph.advance = face->glyph->advance.x >> 6;

		glyphs[character] = glyph;

		delete buffer;

		return true;
	}
	Glyph Font::GetGlypth(FT_UInt character)
	{
		if (glyphs.find(character) != glyphs.end())
		{
			return glyphs[character];
		}
		else 
		{
			
			return LoadGlyph(character) ? glyphs[character] : Glyph();
		}
	}
}
