#include "RawMesh.hpp"
#include "VertexLayout.hpp"
#include "../File.hpp"

BEGIN_INANITY_GRAPHICS

RawMesh::RawMesh(ptr<File> vertices, ptr<VertexLayout> vertexLayout, ptr<File> indices)
: vertices(vertices), vertexLayout(vertexLayout), indices(indices) {}

RawMesh::~RawMesh() {}

ptr<File> RawMesh::GetVertices() const
{
	return vertices;
}

ptr<VertexLayout> RawMesh::GetVertexLayout() const
{
	return vertexLayout;
}

ptr<File> RawMesh::GetIndices() const
{
	return indices;
}

END_INANITY_GRAPHICS
