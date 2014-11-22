#include "FontGlyphs.hpp"

BEGIN_INANITY_GUI

FontGlyphs::FontGlyphs(const GlyphInfos& glyphInfos, int scaleX, int scaleY)
: glyphInfos(glyphInfos), scaleX(scaleX), scaleY(scaleY) {}

const FontGlyphs::GlyphInfos& FontGlyphs::GetGlyphInfos() const
{
	return glyphInfos;
}

int FontGlyphs::GetScaleX() const
{
	return scaleX;
}

int FontGlyphs::GetScaleY() const
{
	return scaleY;
}

END_INANITY_GUI
