#ifndef ___INANITY_GRAPHICS_DX_GEOMETRY_HPP___
#define ___INANITY_GRAPHICS_DX_GEOMETRY_HPP___

#include "Geometry.hpp"

BEGIN_INANITY_GRAPHICS

class DxVertexBuffer;
class DxIndexBuffer;

/// Класс геометрии DirectX.
class DxGeometry : public Geometry
{
private:
	ptr<DxVertexBuffer> vertexBuffer;
	ptr<DxIndexBuffer> indexBuffer;

public:
	DxGeometry(ptr<DxVertexBuffer> vertexBuffer, ptr<DxIndexBuffer> indexBuffer);

	ptr<DxVertexBuffer> GetVertexBuffer() const;
	ptr<DxIndexBuffer> GetIndexBuffer() const;
};

END_INANITY_GRAPHICS

#endif
