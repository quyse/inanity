#include "HbFontShape.hpp"
#include "FontFace.hpp"

BEGIN_INANITY_GUI

using namespace Graphics;

HbFontShape::HbFontShape(float fontSize, ptr<FontFace> face, hb_font_t* font, FT_Face ftFace)
: FontShape(fontSize), face(face), font(font), ftFace(ftFace) {}

HbFontShape::~HbFontShape()
{
	hb_font_destroy(font);
}

void HbFontShape::Shape(const String& text, vec2* outAdvance, std::vector<OutGlyph>* outGlyphs)
{
	FT_Set_Pixel_Sizes(ftFace, (FT_UInt)fontSize, (FT_UInt)fontSize);

	// shape text, and get glyph numbers and offsets
	hb_buffer_t* buffer = hb_buffer_create();
	hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
	hb_buffer_add_utf8(buffer, text.c_str(), text.length(), 0, text.length());
	hb_shape(font, buffer, nullptr, 0);

	unsigned int glyphCount;
	hb_glyph_info_t* glyphInfos = hb_buffer_get_glyph_infos(buffer, &glyphCount);
	hb_glyph_position_t* glyphPositions = hb_buffer_get_glyph_positions(buffer, &glyphCount);

	vec2 minBounds(0, 0), maxBounds(0, 0);

	// process numbers
	vec2 position(0, 0);
	for(unsigned int i = 0; i < glyphCount; ++i)
	{
		// unpack glyph offset and get position
		vec2 glyphPosition = position + vec2(
			float(glyphPositions[i].x_offset) / 64,
			float(glyphPositions[i].y_offset) / 64
		);

		// add out glyph
		if(outGlyphs)
		{
			OutGlyph outGlyph;
			outGlyph.position = glyphPosition;
			outGlyph.number = glyphInfos[i].codepoint;
			outGlyphs->push_back(outGlyph);
		}

		// advance to next glyph
		position.x += float(glyphPositions[i].x_advance) / 64;
		position.y += float(glyphPositions[i].y_advance) / 64;
	}

	if(outAdvance)
		*outAdvance = position;

	// cleanup
	hb_buffer_destroy(buffer);
}

END_INANITY_GUI
