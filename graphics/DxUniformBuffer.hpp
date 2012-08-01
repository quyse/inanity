#ifndef ___INANITY_GRAPHICS_DX_UNIFORM_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX_UNIFORM_BUFFER_HPP___

#include "UniformBuffer.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс uniform-буфера DirectX 11.
class DxUniformBuffer : public UniformBuffer
{
private:
	ComPointer<ID3D11Buffer> buffer;

public:
	DxUniformBuffer(ID3D11Buffer* buffer, size_t size);

	ID3D11Buffer* GetBufferInterface() const;
};

END_INANITY_GRAPHICS

#endif
