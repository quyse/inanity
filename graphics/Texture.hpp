#ifndef ___INANITY_GRAPHICS_TEXTURE_HPP___
#define ___INANITY_GRAPHICS_TEXTURE_HPP___

#include "graphics.hpp"
#include "../scripting_decl.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс текстуры.
/** Текстура - это нечто, которое может быть привязано
к текстурному слоту в шейдере. */
class Texture : public Object
{
public:
	SCRIPTABLE_CLASS(Texture);
};

END_INANITY_GRAPHICS

#endif
