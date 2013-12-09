#include "Font.hpp"
#include "FontGlyphs.hpp"
#include "Canvas.hpp"

BEGIN_INANITY_GUI

Font::Font(ptr<FontShape> shape, ptr<FontGlyphs> glyphs)
: shape(shape), glyphs(glyphs) {}

ptr<FontShape> Font::GetShape() const
{
	return shape;
}

ptr<FontGlyphs> Font::GetGlyphs() const
{
	return glyphs;
}

void Font::DrawString(Canvas* canvas, const String& text, const Graphics::vec2& position)
{
	outGlyphs.clear();
	shape->Shape(text, nullptr, &outGlyphs);

	for(size_t i = 0; i < outGlyphs.size(); ++i)
		canvas->DrawGlyph(glyphs, outGlyphs[i].number, position + outGlyphs[i].position);
}

END_INANITY_GUI
