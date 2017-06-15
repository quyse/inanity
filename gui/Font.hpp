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
public:
	enum TextOriginFlags
	{
		textOriginHorizontalMask = 0x03,
		textOriginPen = 0x00,
		textOriginLeft = 0x01,
		textOriginCenter = 0x02,
		textOriginRight = 0x03,

		textOriginVerticalMask = 0x0C,
		textOriginBaseline = 0x00,
		textOriginTop = 0x04,
		textOriginMiddle = 0x08,
		textOriginBottom = 0x0C
	};

private:
	ptr<FontShape> shape;
	ptr<FontGlyphs> glyphs;
	std::vector<FontShape::OutGlyph> outGlyphs;
	/// Bounds of shaped glyphs.
	float left, top, right, bottom;

	void InternalShape(const String& text, FontShape::Script script);

public:
	Font(ptr<FontShape> shape, ptr<FontGlyphs> glyphs);

	ptr<FontShape> GetShape() const;
	ptr<FontGlyphs> GetGlyphs() const;

	void DrawString(
		Canvas* canvas,
		const String& text,
		FontShape::Script script,
		const Graphics::vec2& position,
		const Graphics::vec4& color,
		int textOriginFlags = textOriginPen | textOriginBaseline);

	Graphics::vec2 GetStringSize(const String& text, FontShape::Script script);
};

END_INANITY_GUI

#endif
