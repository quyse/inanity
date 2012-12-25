#ifndef ___INANITY_GRAPHICS_DX_INDEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX_INDEX_BUFFER_HPP___

#include "IndexBuffer.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс индексного буфера DirectX.
class DxIndexBuffer : public IndexBuffer
{
private:
	ComPointer<ID3D11Buffer> buffer;

public:
	DxIndexBuffer(ComPointer<ID3D11Buffer> buffer, size_t indicesCount);

	ID3D11Buffer* GetBufferInterface() const;
};

END_INANITY_GRAPHICS

#endif
