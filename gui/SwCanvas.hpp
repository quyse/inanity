#ifndef ___INANITY_GUI_SW_CANVAS_HPP___
#define ___INANITY_GUI_SW_CANVAS_HPP___

#include "Canvas.hpp"
#include <vector>

BEGIN_INANITY_GUI

/// Software canvas class.
class SwCanvas : public Canvas
{
private:
	ptr<Graphics::RawTextureData> destination;

public:
	void SetDestination(ptr<Graphics::RawTextureData> destination);

	//*** Canvas' methods.
	ptr<FontGlyphs> CreateGlyphs(
		ptr<Graphics::RawTextureData> image,
		const FontGlyphs::GlyphInfos& glyphInfos,
		int scaleX,
		int scaleY
	);
	void DrawGlyph(FontGlyphs* glyphs, int glyphIndex, const Graphics::vec2& penPoint, const Graphics::vec4& color);
};

END_INANITY_GUI

#endif
