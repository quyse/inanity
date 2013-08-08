#ifndef ___INANITY_GRAPHICS_DX11_INDEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX11_INDEX_BUFFER_HPP___

#include "IndexBuffer.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс индексного буфера DirectX.
class Dx11IndexBuffer : public IndexBuffer
{
private:
	ComPointer<ID3D11Buffer> buffer;

public:
	Dx11IndexBuffer(ComPointer<ID3D11Buffer> buffer, int indicesCount, int indexSize);

	ID3D11Buffer* GetBufferInterface() const;

	DXGI_FORMAT GetIndexFormat() const;
};

END_INANITY_GRAPHICS

#endif
