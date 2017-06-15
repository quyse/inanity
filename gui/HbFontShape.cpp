#include "HbFontShape.hpp"
#include "FontFace.hpp"

BEGIN_INANITY_GUI

using namespace Graphics;

HbFontShape::HbFontShape(ptr<FontFace> face, hb_font_t* font)
: face(face), font(font), buffer(hb_buffer_create()) {}

HbFontShape::~HbFontShape()
{
	hb_font_destroy(font);
	hb_buffer_destroy(buffer);
}

void HbFontShape::Shape(const String& text, Script script, vec2* outAdvance, std::vector<OutGlyph>* outGlyphs)
{
	// shape text, and get glyph numbers and offsets
	hb_buffer_set_script(buffer, (hb_script_t)script);
	hb_buffer_set_direction(buffer, hb_script_get_horizontal_direction((hb_script_t)script));
	hb_buffer_add_utf8(buffer, text.c_str(), (int)text.length(), 0, (int)text.length());
	hb_shape(font, buffer, nullptr, 0);

	unsigned int glyphCount;
	hb_glyph_info_t* glyphInfos = hb_buffer_get_glyph_infos(buffer, &glyphCount);
	hb_glyph_position_t* glyphPositions = hb_buffer_get_glyph_positions(buffer, &glyphCount);

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
	hb_buffer_clear_contents(buffer);
}

END_INANITY_GUI
