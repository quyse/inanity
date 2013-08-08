#ifndef ___INANITY_GRAPHICS_FONT_ALIGNMENT_HPP___
#define ___INANITY_GRAPHICS_FONT_ALIGNMENT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Выравнивание для шрифта.
/** Задаёт сразу горизонтальное и вертикальное выравнивание. */
struct FontAlignments
{
	enum _
	{
		HorizontalMask = 0x3,
		Left = 0x0,
		Center = 0x1,
		Right = 0x2,
		VerticalMask = 0xC,
		Top = 0x8,
		Middle = 0x4,
		Bottom = 0x0
	};
};
typedef FontAlignments::_ FontAlignment;

END_INANITY_GRAPHICS

#endif
