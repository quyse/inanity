#include "FontGlyphs.hpp"

BEGIN_INANITY_GUI

FontGlyphs::FontGlyphs(const GlyphInfos& glyphInfos)
: glyphInfos(glyphInfos) {}

const FontGlyphs::GlyphInfos& FontGlyphs::GetGlyphInfos() const
{
	return glyphInfos;
}

END_INANITY_GUI
