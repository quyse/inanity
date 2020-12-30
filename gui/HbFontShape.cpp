#include "HbFontShape.hpp"
#include "FontFace.hpp"
#include "../deps/icu/repo/source/common/unicode/brkiter.h"
#include "../deps/icu/repo/source/common/unicode/ubidi.h"
#include "../deps/icu/repo/source/common/unicode/utext.h"

BEGIN_INANITY_GUI

using namespace Graphics;

HbFontShape::HbFontShape(ptr<FontFace> face, hb_font_t* font)
: face(face), font(font), buffer(hb_buffer_create()) {}

HbFontShape::~HbFontShape()
{
	hb_font_destroy(font);
	hb_buffer_destroy(buffer);
}

FontShape::Language HbFontShape::GetLanguage(const String& tag)
{
	return (Language)hb_language_from_string(tag.c_str(), (int)tag.length());
}

void HbFontShape::Shape(const String& text, Script script, Language language, vec2* outAdvance, std::vector<OutGlyph>* outGlyphs)
{
	if(text.empty())
	{
		if(outAdvance)
			*outAdvance = vec2(0.0f, 0.0f);
		return;
	}

	// shape text, and get glyph numbers and offsets
	hb_buffer_set_script(buffer, (hb_script_t)script);
	hb_buffer_set_language(buffer, (hb_language_t)language);
	hb_buffer_set_direction(buffer, hb_script_get_horizontal_direction((hb_script_t)script));
	hb_buffer_add_utf8(buffer, text.c_str(), (int)text.length(), 0, (int)text.length());
	hb_shape(font, buffer, nullptr, 0);
	hb_buffer_normalize_glyphs(buffer);

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

		vec2 glyphAdvance(float(glyphPositions[i].x_advance) / 64, float(glyphPositions[i].y_advance) / 64);

		// add out glyph
		if(outGlyphs)
		{
			OutGlyph outGlyph;
			outGlyph.position = glyphPosition;
			outGlyph.advance = glyphAdvance;
			outGlyph.glyphIndex = glyphInfos[i].codepoint;
			outGlyph.characterIndex = glyphInfos[i].cluster;
			outGlyphs->push_back(outGlyph);
		}

		// advance to next glyph
		position += glyphAdvance;
	}

	if(outAdvance)
		*outAdvance = position;

	// cleanup
	hb_buffer_reset(buffer);
}

void HbFontShape::ShapeParagraph(const String& text, Script script, Language language, const icu::Locale& locale, float maxLineWidth, float lineHeight, std::vector<OutGlyph>* outGlyphs)
{
	UErrorCode e = U_ZERO_ERROR;
	icu::UnicodeString s = icu::UnicodeString::fromUTF8(text);
	UBiDi* ubidi = ubidi_openSized(s.length(), 0, &e);
	if(!ubidi) return;

	UBiDiLevel paragraphDirection = hb_script_get_horizontal_direction((hb_script_t)script) == HB_DIRECTION_RTL ? UBIDI_DEFAULT_RTL : UBIDI_DEFAULT_LTR;
	ubidi_setPara(ubidi, s.getBuffer(), s.length(), paragraphDirection, nullptr, &e);

	float currentLineWidth = 0;
	Graphics::vec2 linePosition(0, 0);

	// break iterator for breaking logical runs into pieces
	icu::BreakIterator* breakIter = icu::BreakIterator::createLineInstance(locale, e);

	// loop for logical runs
	int32_t runLimit;
	UBiDiLevel runLevel;
	for(int32_t runStart = 0; ubidi_getLogicalRun(ubidi, runStart, &runLimit, &runLevel), runStart < s.length(); runStart = runLimit)
	{
		// break logical run into pieces
		icu::UnicodeString runStr = s.tempSubStringBetween(runStart, runLimit);
		breakIter->setText(runStr);

		// shape logical run
		hb_buffer_set_script(buffer, (hb_script_t)script);
		hb_buffer_set_language(buffer, (hb_language_t)language);
		hb_buffer_set_direction(buffer, runLevel == UBIDI_RTL ? HB_DIRECTION_RTL : HB_DIRECTION_LTR);
		hb_buffer_add_utf16(buffer, (const uint16_t*)runStr.getBuffer(), runStr.length(), 0, runStr.length());
		hb_shape(font, buffer, nullptr, 0);
		hb_buffer_normalize_glyphs(buffer);

		unsigned int glyphCount;
		hb_glyph_info_t* glyphInfos = hb_buffer_get_glyph_infos(buffer, &glyphCount);
		hb_glyph_position_t* glyphPositions = hb_buffer_get_glyph_positions(buffer, &glyphCount);

		auto addCombinedPiece = [&](int32_t pieceStart, int32_t pieceLimit, float pieceWidth)
		{
			if(pieceStart >= pieceLimit) return;

			// if it doesn't fit into current line (and not the first piece in a line)
			float newCurrentLineWidth = currentLineWidth + pieceWidth;
			if(currentLineWidth > 0 && newCurrentLineWidth > maxLineWidth)
			{
				// start new line
				currentLineWidth = 0;
				linePosition.x = 0;
				linePosition.y += lineHeight;
			}

			// add piece to current line
			currentLineWidth += pieceWidth;

			if(paragraphDirection == UBIDI_DEFAULT_RTL) linePosition.x -= pieceWidth;
			Graphics::vec2 position = linePosition;
			if(paragraphDirection != UBIDI_DEFAULT_RTL) linePosition.x += pieceWidth;

			for(unsigned int i = 0; i < glyphCount; ++i)
			{
				if((int32_t)glyphInfos[i].cluster >= pieceStart && (int32_t)glyphInfos[i].cluster < pieceLimit)
				{
					// unpack glyph offset and get position
					vec2 glyphPosition = position + vec2(
						float(glyphPositions[i].x_offset) / 64,
						float(glyphPositions[i].y_offset) / 64
					);

					vec2 glyphAdvance(float(glyphPositions[i].x_advance) / 64, float(glyphPositions[i].y_advance) / 64);

					// add out glyph
					if(outGlyphs)
					{
						OutGlyph outGlyph;
						outGlyph.position = glyphPosition;
						outGlyph.advance = glyphAdvance;
						outGlyph.glyphIndex = glyphInfos[i].codepoint;
						outGlyph.characterIndex = glyphInfos[i].cluster;
						outGlyphs->push_back(outGlyph);
					}

					// advance to next glyph
					position += glyphAdvance;
				}
			}
		};

		int32_t pieceLimit;
		int32_t combinedPieceStart = 0;
		float combinedPieceWidth = 0;
		for(int32_t pieceStart = 0; pieceStart < runStr.length(); pieceStart = pieceLimit)
		{
			pieceLimit = breakIter->next();
			if(pieceLimit == icu::BreakIterator::DONE)
			{
				pieceLimit = runStr.length();
			}

			// calculate width of a piece
			float pieceWidth = 0;
			for(unsigned int i = 0; i < glyphCount; ++i)
			{
				if((int32_t)glyphInfos[i].cluster >= pieceStart && (int32_t)glyphInfos[i].cluster < pieceLimit)
				{
					pieceWidth += float(glyphPositions[i].x_advance) / 64;
				}
			}

			// try to combine it
			float newCombinedPieceWidth = combinedPieceWidth + pieceWidth;
			// if there's no overflow
			if(currentLineWidth + newCombinedPieceWidth <= maxLineWidth)
			{
				combinedPieceWidth = newCombinedPieceWidth;
			}
			// else it overflows
			else
			{
				// flush combined piece
				addCombinedPiece(combinedPieceStart, pieceStart, combinedPieceWidth);
				combinedPieceStart = pieceStart;
				combinedPieceWidth = pieceWidth;
				// ensure we start new line
				if(currentLineWidth > 0)
				{
					currentLineWidth = 0;
					linePosition.x = 0;
					linePosition.y += lineHeight;
				}
			}
		}
		// flush last combined piece
		addCombinedPiece(combinedPieceStart, runStr.length(), combinedPieceWidth);

		// cleanup
		hb_buffer_reset(buffer);
	};

	delete breakIter;

	ubidi_close(ubidi);
}

END_INANITY_GUI
