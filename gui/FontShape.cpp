#include "FontShape.hpp"

BEGIN_INANITY_GUI

FontShape::FontShape(float fontSize)
: fontSize(fontSize) {}

float FontShape::GetFontSize() const
{
	return fontSize;
}

END_INANITY_GUI
