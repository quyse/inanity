#include "Geometry.hpp"
#include "GeometryFormat.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

Geometry::Geometry(ptr<File> vertices, size_t vertexStride, ptr<File> indices, size_t indexStride, ptr<GeometryFormat> format)
: vertices(vertices), vertexStride(vertexStride), indices(indices), indexStride(indexStride), format(format)
{
	if(!indices) Geometry::indexStride = 0;
}

ptr<File> Geometry::GetVertices() const
{
	return vertices;
}

size_t Geometry::GetVertexStride() const
{
	return vertexStride;
}

ptr<File> Geometry::GetIndices() const
{
	return indices;
}

size_t Geometry::GetIndexStride() const
{
	return indexStride;
}

ptr<GeometryFormat> Geometry::GetFormat() const
{
	return format;
}
