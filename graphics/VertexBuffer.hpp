#ifndef ___INANITY_GRAPHICS_VERTEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_VERTEX_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс вершинного буфера.
class VertexBuffer : public Object
{
protected:
	size_t verticesCount;

protected:
	VertexBuffer(size_t verticesCount);

public:
	size_t GetVerticesCount() const;
};

END_INANITY_GRAPHICS

#endif
