#include "D3DBlobFile.hpp"

BEGIN_INANITY_GRAPHICS

D3DBlobFile::D3DBlobFile(ID3DBlob* blob) : blob(blob) {}

void* D3DBlobFile::GetData() const
{
	return blob->GetBufferPointer();
}

size_t D3DBlobFile::GetSize() const
{
	return blob->GetBufferSize();
}

END_INANITY_GRAPHICS
