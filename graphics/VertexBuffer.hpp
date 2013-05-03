#ifndef ___INANITY_GRAPHICS_VERTEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_VERTEX_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class VertexLayout;

/// Абстрактный класс вершинного буфера.
class VertexBuffer : public Object
{
protected:
	int verticesCount;
	ptr<VertexLayout> layout;

protected:
	VertexBuffer(int verticesCount, ptr<VertexLayout> layout);

public:
	int GetVerticesCount() const;

	ptr<VertexLayout> GetLayout() const;
};

END_INANITY_GRAPHICS

#endif
