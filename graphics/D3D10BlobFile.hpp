#ifndef ___INANITY_GRAPHICS_D3D10_BLOB_FILE_HPP___
#define ___INANITY_GRAPHICS_D3D10_BLOB_FILE_HPP___

#include "graphics.hpp"
#include "d3d.hpp"
#include "../File.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс файла, инкапсулирующего в себе объект интерфейса ID3D10Blob.
class D3D10BlobFile : public File
{
private:
	ComPointer<ID3D10Blob> blob;

public:
	D3D10BlobFile(ID3D10Blob* blob);

	void* GetData() const;
	size_t GetSize() const;
};

END_INANITY_GRAPHICS

#endif
