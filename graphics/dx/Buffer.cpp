#include "Buffer.hpp"
#include "../../Exception.hpp"

Buffer::Buffer(ID3D11Buffer* buffer, size_t size) : buffer(buffer), size(size)
{
}

ID3D11Buffer* Buffer::GetInternalInterface() const
{
	return buffer;
}

size_t Buffer::GetSize() const
{
	return size;
}
