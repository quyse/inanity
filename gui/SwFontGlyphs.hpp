#ifndef ___INANITY_GUI_SW_FONT_GLYPHS_HPP___
#define ___INANITY_GUI_SW_FONT_GLYPHS_HPP___

#include "FontGlyphs.hpp"
#include "../graphics/graphics.hpp"

BEGIN_INANITY_GRAPHICS

class RawTextureData;

END_INANITY_GRAPHICS

BEGIN_INANITY_GUI

class SwFontGlyphs : public FontGlyphs
{
private:
	ptr<Graphics::RawTextureData> image;
	GlyphInfos glyphInfos;

public:
	SwFontGlyphs(ptr<Graphics::RawTextureData> image, const GlyphInfos& glyphInfos);

	ptr<Graphics::RawTextureData> GetImage() const;
	const GlyphInfos& GetGlyphInfos() const;
};

END_INANITY_GUI

#endif
