#ifndef ___INANITY_GRAPHICS_TEXTURE_HPP___
#define ___INANITY_GRAPHICS_TEXTURE_HPP___

#include "graphics.hpp"
#include "../meta/decl.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс текстуры.
/** Текстура - это нечто, которое может быть привязано
к текстурному слоту в шейдере. */
class Texture : public Object
{
public:
	META_DECLARE_CLASS(Texture);

	int GetWidth() const
	{
		return width;
	}

	int GetHeight() const
	{
		return height;
	}

	int GetDepth() const
	{
		return depth;
	}

protected:
	Texture(int width, int height, int depth)
	: width(width), height(height), depth(depth) {}

	int width, height, depth;
};

END_INANITY_GRAPHICS

#endif
