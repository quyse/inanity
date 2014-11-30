#ifndef ___INANITY_GRAPHICS_DX11_UNIFORM_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX11_UNIFORM_BUFFER_HPP___

#include "UniformBuffer.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс uniform-буфера DirectX 11.
class Dx11UniformBuffer : public UniformBuffer
{
private:
	ComPointer<ID3D11Buffer> buffer;

public:
	Dx11UniformBuffer(ComPointer<ID3D11Buffer> buffer, int size);

	ID3D11Buffer* GetBufferInterface() const;
};

END_INANITY_GRAPHICS

#endif
