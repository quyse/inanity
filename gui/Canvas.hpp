#ifndef ___INANITY_GUI_CANVAS_HPP___
#define ___INANITY_GUI_CANVAS_HPP___

#include "FontGlyphs.hpp"
#include "FontImageType.hpp"
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
		const FontGlyphs::GlyphInfos& glyphInfos,
		float originalFontSize,
		FontImageType fontImageType,
		float smoothCoef,
		ptr<Graphics::RawTextureData> image
		) = 0;
	/// Draw glyph.
	virtual void DrawGlyph(
		FontGlyphs* glyphs,
		int glyphIndex,
		const Graphics::vec2& penPoint,
		float scale,
		const Graphics::vec4& color
	) = 0;
	/// Flush pending draws.
	/** Default implementation do nothing. */
	virtual void Flush();
};

END_INANITY_GUI

#endif
