#ifndef ___INANITY_GUI_HB_FONT_SHAPE_HPP___
#define ___INANITY_GUI_HB_FONT_SHAPE_HPP___

#include "FontShape.hpp"
#include "hb.hpp"
#include "ft.hpp"

BEGIN_INANITY_GUI

class FontFace;

/// Font shape for HarfBuzz text shaping.
class HbFontShape : public FontShape
{
private:
	ptr<FontFace> face;
	hb_font_t* font;
	FT_Face ftFace;

public:
	HbFontShape(float fontSize, ptr<FontFace> face, hb_font_t* font, FT_Face ftFace);
	~HbFontShape();

	/// FontShape's methods.
	/** Out parameters are optional. */
	void Shape(const String& text, Graphics::vec2* outSize, std::vector<OutGlyph>* outGlyphs);
};

END_INANITY_GUI

#endif
