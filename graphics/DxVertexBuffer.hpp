#ifndef ___INANITY_GRAPHICS_DX_VERTEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX_VERTEX_BUFFER_HPP___

#include "VertexBuffer.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class Layout;

/// Класс вершинного буфера DirectX.
class DxVertexBuffer : public VertexBuffer
{
private:
	ComPointer<ID3D11Buffer> buffer;

public:
	DxVertexBuffer(ComPointer<ID3D11Buffer> buffer, int verticesCount, ptr<Layout> layout);

	ID3D11Buffer* GetBufferInterface() const;
};

END_INANITY_GRAPHICS

#endif
