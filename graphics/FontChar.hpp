#ifndef ___INANITY_FONT_CHAR_HPP___
#define ___INANITY_FONT_CHAR_HPP___

#include "graphics.hpp"
#include "../inanity-math.hpp"

BEGIN_INANITY_GRAPHICS

/*
Файл содержит структуру символа в шрифте.
Замечание по поводу экранных размеров. Рекомендуется задавать их в пикселах.
А при рисовании задавать scaleX и scaleY равными 2 / размер экрана в пикселах.
Ну, или отталкиваться от этого.
*/

//структура символа в наборе
struct FontChar
{
	//текстурные координаты левого нижнего угла
	float2 firstUV;
	//текстурные координаты правого верхнего угла
	float2 secondUV;

	//смещение на экране левого нижнего угла (в пикселах)
	float2 screenFirstUV;
	//смещение на экране правого верхнего угла (в пикселах)
	float2 screenSecondUV;

	//логическая ширина символа
	float width;

	FontChar() : firstUV(0, 0), screenFirstUV(0, 0), screenSecondUV(0, 0), width(0)
	{
	}
};

END_INANITY_GRAPHICS

#endif
