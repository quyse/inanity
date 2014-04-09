#include "Font.hpp"
#include "FontGlyphs.hpp"
#include "Canvas.hpp"
#include "../Exception.hpp"
#include <limits>

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

void Font::InternalShape(const String& text)
{
	outGlyphs.clear();
	Graphics::vec2 size;
	shape->Shape(text, &size, &outGlyphs);

	// calculate string bounds
	const FontGlyphs::GlyphInfos& glyphInfos = glyphs->GetGlyphInfos();

	if(outGlyphs.empty())
		left = right = top = bottom = 0;
	else
	{
		left = std::numeric_limits<float>::max();
		right = std::numeric_limits<float>::min();
		top = std::numeric_limits<float>::max();
		bottom = std::numeric_limits<float>::min();

		for(size_t i = 0; i < outGlyphs.size(); ++i)
		{
			const FontGlyphs::GlyphInfo& glyphInfo = glyphInfos[outGlyphs[i].number];
			const Graphics::vec2& glyphPosition = outGlyphs[i].position;
			left = std::min(left, glyphPosition.x + glyphInfo.offsetX);
			right = std::max(right, glyphPosition.x + glyphInfo.offsetX + glyphInfo.width);
			top = std::min(top, glyphPosition.y + glyphInfo.offsetY);
			bottom = std::max(bottom, glyphPosition.y + glyphInfo.offsetY + glyphInfo.height);
		}
	}
}

void Font::DrawString(
	Canvas* canvas,
	const String& text,
	const Graphics::vec2& position,
	const Graphics::vec4& color,
	int textOriginFlags)
{
	BEGIN_TRY();

	// shape glyphs
	InternalShape(text);

	// calculate origin point
	Graphics::vec2 origin = position;
	// horizontal
	switch(textOriginFlags & textOriginHorizontalMask)
	{
	case textOriginPen:
		// nothing has to be added
		break;
	case textOriginLeft:
		origin.x -= left;
		break;
	case textOriginCenter:
		origin.x -= (left + right) * 0.5f;
		break;
	case textOriginRight:
		origin.x -= right;
		break;
	default:
		THROW("Invalid horizontal alignment");
	}
	// vertical
	switch(textOriginFlags & textOriginVerticalMask)
	{
	case textOriginBaseline:
		// nothing has to be added
		break;
	case textOriginTop:
		origin.y -= top;
		break;
	case textOriginMiddle:
		origin.y -= (top + bottom) * 0.5f;
		break;
	case textOriginBottom:
		origin.y -= bottom;
		break;
	default:
		THROW("Invalid vertical alignment");
	}

	for(size_t i = 0; i < outGlyphs.size(); ++i)
		canvas->DrawGlyph(glyphs, outGlyphs[i].number, origin + outGlyphs[i].position, color);

	END_TRY("Can't draw string with font");
}

Graphics::vec2 Font::GetStringSize(const String& text)
{
	InternalShape(text);
	return Graphics::vec2(right - left, bottom - top);
}

END_INANITY_GUI
