#ifndef ___INANITY_GUI_FONT_SHAPE_HPP___
#define ___INANITY_GUI_FONT_SHAPE_HPP___

#include "gui.hpp"
#include "../graphics/graphics.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_GUI

/// Font shape class.
/** Contains text shaping rules for some font (or several),
for specific font size. */
class FontShape : public Object
{
public:
	/// Output glyph for text shape.
	struct OutGlyph
	{
		Graphics::vec2 position;
		int number;
	};

public:
	virtual void Shape(const String& text, Graphics::vec2* outSize, std::vector<OutGlyph>* outGlyphs) = 0;
};

END_INANITY_GUI

#endif
