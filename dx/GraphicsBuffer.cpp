#include "GraphicsBuffer.h"
#include "Exception.h"

GraphicsBuffer::GraphicsBuffer(ID3D11Buffer* buffer, size_t size) : buffer(buffer), size(size)
{
}

ID3D11Buffer* GraphicsBuffer::GetInternalInterface() const
{
	return buffer;
}

size_t GraphicsBuffer::GetSize() const
{
	return size;
}
