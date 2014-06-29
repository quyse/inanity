#ifndef ___INANITY_GRAPHICS_RAW_MESH_HPP___
#define ___INANITY_GRAPHICS_RAW_MESH_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class VertexLayout;

class RawMesh : public Object
{
private:
	ptr<File> vertices;
	ptr<VertexLayout> vertexLayout;
	ptr<File> indices;

public:
	RawMesh(ptr<File> vertices, ptr<VertexLayout> vertexLayout, ptr<File> indices);
	~RawMesh();

	ptr<File> GetVertices() const;
	ptr<VertexLayout> GetVertexLayout() const;
	ptr<File> GetIndices() const;
};

END_INANITY_GRAPHICS

#endif
