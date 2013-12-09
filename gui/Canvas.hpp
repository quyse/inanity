#ifndef ___INANITY_GUI_CANVAS_HPP___
#define ___INANITY_GUI_CANVAS_HPP___

#include "FontGlyphs.hpp"
#include "../graphics/graphics.hpp"

BEGIN_INANITY_GRAPHICS

class RawTextureData;

END_INANITY_GRAPHICS

BEGIN_INANITY_GUI

/// Canvas abstract class.
/** Provides methods to draw 2D with respect to pixels. */
class Canvas : public Object
{
public:
	/// Create font glyphs in appropriate format.
	virtual ptr<FontGlyphs> CreateGlyphs(
		ptr<Graphics::RawTextureData> image,
		const FontGlyphs::GlyphInfos& glyphInfos
		) = 0;
	/// Draw glyph.
	virtual void DrawGlyph(FontGlyphs* glyphs, int glyphIndex, const Graphics::vec2& penPoint) = 0;
};

END_INANITY_GUI

#endif
