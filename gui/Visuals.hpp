#ifndef ___INANITY_GUI_VISUALS_HPP___
#define ___INANITY_GUI_VISUALS_HPP___

#include "gui.hpp"

BEGIN_INANITY_GUI

/// Структура с перечислением типов визуальных элементов.
struct VisualElements
{
	enum _
	{
		Button,
		ButtonHighlighted,
		ButtonPressed,
		TextBox,
		TextBoxHighlighted,
		Caret,
		Selection,
		SliderLine,
		ProgressFrame,
		ProgressBar,
		Count
	};
};
typedef VisualElements::_ VisualElement;

/// Структура с перечислением шрифтов визуальных элементов.
/** Шрифт задаёт не только собственно шрифт, но и побочные
настройки, такие, как цвет и выравнивание. */
struct VisualFonts
{
	enum _
	{
		Label,
		UserText,
		Button,
		ButtonHighlighted,
		ButtonPressed,
		Count
	};
};
typedef VisualFonts::_ VisualFont;

/// Структура с перечислением визуальных метрик.
struct VisualMetrics
{
	Distance textBoxTextX;
	Size sliderHalf;
	Distance progressBarX;
	Position windowClientLeftTop;
	Position windowClientRightBottom;
};

END_INANITY_GUI

#endif
