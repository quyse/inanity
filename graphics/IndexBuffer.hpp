#ifndef ___INANITY_GRAPHICS_INDEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_INDEX_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс индексного буфера.
class IndexBuffer : public Object
{
protected:
	/// Количество индексов.
	int indicesCount;
	/// Размер одного индекса - 2 или 4.
	int indexSize;

protected:
	IndexBuffer(int indicesCount, int indexSize);

public:
	/// Получить количество индексов в буфере.
	int GetIndicesCount() const;
	/// Получить размер одного индекса.
	int GetIndexSize() const;
};

END_INANITY_GRAPHICS

#endif
