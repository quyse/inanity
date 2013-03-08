#ifndef ___INANITY_GRAPHICS_VERTEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_VERTEX_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Layout;

/// Абстрактный класс вершинного буфера.
class VertexBuffer : public Object
{
protected:
	int verticesCount;
	ptr<Layout> layout;

protected:
	VertexBuffer(int verticesCount, ptr<Layout> layout);

public:
	int GetVerticesCount() const;

	ptr<Layout> GetLayout() const;
};

END_INANITY_GRAPHICS

#endif
