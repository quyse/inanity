#ifndef ___INANITY_GRAPHICS_DX11_VERTEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX11_VERTEX_BUFFER_HPP___

#include "VertexBuffer.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс вершинного буфера DirectX.
class Dx11VertexBuffer : public VertexBuffer
{
private:
	ComPointer<ID3D11Buffer> buffer;

public:
	Dx11VertexBuffer(ComPointer<ID3D11Buffer> buffer, int verticesCount, ptr<VertexLayout> layout);

	ID3D11Buffer* GetBufferInterface() const;
};

END_INANITY_GRAPHICS

#endif
