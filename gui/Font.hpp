#ifndef ___INANITY_GUI_FONT_HPP___
#define ___INANITY_GUI_FONT_HPP___

#include "FontShape.hpp"
#include "../graphics/graphics.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

class FontGlyphs;
class Canvas;

/// Class combines FontShape and FontGlyphs.
class Font : public Object
{
private:
	ptr<FontShape> shape;
	ptr<FontGlyphs> glyphs;
	std::vector<FontShape::OutGlyph> outGlyphs;

public:
	Font(ptr<FontShape> shape, ptr<FontGlyphs> glyphs);

	ptr<FontShape> GetShape() const;
	ptr<FontGlyphs> GetGlyphs() const;

	void DrawString(Canvas* canvas, const String& text, const Graphics::vec2& position);
};

END_INANITY_GUI

#endif
