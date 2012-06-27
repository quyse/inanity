#include "ConstantBuffer.hpp"

ConstantBuffer::ConstantBuffer(ID3D11Buffer* buffer, size_t size)
	: Buffer(buffer, size)
{
}
