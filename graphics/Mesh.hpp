#ifndef ___INANITY_GRAPHICS_MESH_HPP___
#define ___INANITY_GRAPHICS_MESH_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class VertexBuffer;
class IndexBuffer;

/// Class combining vertex and index buffer.
class Mesh : public Object
{
private:
	ptr<VertexBuffer> vertexBuffer;
	ptr<IndexBuffer> indexBuffer;

public:
	Mesh(ptr<VertexBuffer> vertexBuffer, ptr<IndexBuffer> indexBuffer);
	~Mesh();

	ptr<VertexBuffer> GetVertexBuffer() const;
	ptr<IndexBuffer> GetIndexBuffer() const;
};

END_INANITY_GRAPHICS

#endif
