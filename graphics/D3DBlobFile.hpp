#ifndef ___INANITY_GRAPHICS_D3D_BLOB_FILE_HPP___
#define ___INANITY_GRAPHICS_D3D_BLOB_FILE_HPP___

#include "graphics.hpp"
#include "d3d11.hpp"
#include "../File.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс файла, инкапсулирующего в себе объект интерфейса ID3DBlob.
class D3DBlobFile : public File
{
private:
	ComPointer<ID3DBlob> blob;

public:
	D3DBlobFile(ID3DBlob* blob);

	void* GetData() const;
	size_t GetSize() const;
};

END_INANITY_GRAPHICS

#endif
