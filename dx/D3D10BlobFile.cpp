#include "D3D10BlobFile.hpp"

D3D10BlobFile::D3D10BlobFile(ID3D10Blob* blob) : blob(blob)
{
}

void* D3D10BlobFile::GetData() const
{
	return blob->GetBufferPointer();
}

size_t D3D10BlobFile::GetSize() const
{
	return blob->GetBufferSize();
}
