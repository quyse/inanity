#ifndef ___INANITY_GRAPHICS_INDEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_INDEX_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс индексного буфера.
class IndexBuffer : public Object
{
protected:
	size_t indicesCount;

	IndexBuffer(size_t indicesCount);

public:
	/// Получить количество индексов в буфере.
	size_t GetIndicesCount() const;
};

END_INANITY_GRAPHICS

#endif
