#ifndef ___INANITY_GRAPHICS_UNIFORM_BUFFER_HPP___
#define ___INANITY_GRAPHICS_UNIFORM_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс буфера константных данных для шейдера.
/** В DirectX это называется константный буфер,
в OpenGl - uniform-буфер. */
class UniformBuffer : public Object
{
public:
	/// Получить размер буфера.
	/** Размер буфера указывается при создании, и
	не может быть изменён. */
	virtual size_t GetSize() = 0;
};

END_INANITY_GRAPHICS

#endif
