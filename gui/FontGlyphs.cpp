#include "FontGlyphs.hpp"

BEGIN_INANITY_GUI

FontGlyphs::FontGlyphs(const GlyphInfos& glyphInfos, float originalFontSize, FontImageType fontImageType, float smoothCoef)
: glyphInfos(glyphInfos), originalFontSize(originalFontSize), fontImageType(fontImageType), smoothCoef(smoothCoef) {}

const FontGlyphs::GlyphInfos& FontGlyphs::GetGlyphInfos() const
{
	return glyphInfos;
}

float FontGlyphs::GetOriginalFontSize() const
{
	return originalFontSize;
}

FontImageType FontGlyphs::GetFontImageType() const
{
	return fontImageType;
}

float FontGlyphs::GetSmoothCoef() const
{
	return smoothCoef;
}

END_INANITY_GUI
