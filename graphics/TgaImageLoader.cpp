#include "TgaImageLoader.hpp"
#include "RawTextureData.hpp"
#include "../File.hpp"
#include "../Exception.hpp"
#include <cstdint>

BEGIN_INANITY_GRAPHICS

ptr<RawTextureData> TgaImageLoader::Load(ptr<File> file)
{
#pragma pack(push, 1)
	struct Header
	{
		uint8_t idLength;
		uint8_t colorMapType;
		uint8_t imageType;
		uint8_t colorMapSpec[5];
		uint16_t xOrigin;
		uint16_t yOrigin;
		uint16_t width;
		uint16_t height;
		uint8_t bitsPerPixel;
		uint8_t imageDesc;
	};
#pragma pack(pop)

	BEGIN_TRY();

	const uint8_t* fileData = (const uint8_t*)file->GetData();
	size_t fileSize = file->GetSize();

	// read header
	if(fileSize < sizeof(Header))
		THROW("Can't read header");
	const Header* header = (const Header*)fileData;

	// check that there is no color map
	if(header->colorMapType != 0)
		THROW("Unsupported color map type");

	// for now we support only uncompressed images
	if(header->imageType != 2)
		THROW("Unsupported image type");

	// check bits per pixel
	if(header->bitsPerPixel != 24)
		THROW("Unsupported bits per pixel value");

	// check size of pixel data
	int width = (int)header->width;
	int height = (int)header->height;
	int pitch = width * (header->bitsPerPixel / 8);
	const uint8_t* pixelData = (const uint8_t*)(header + 1);
	if(fileSize - sizeof(Header) < (size_t)(width * pitch))
		THROW("Wrong size of file");

	// create image data
	ptr<RawTextureData> textureData = NEW(RawTextureData(
		nullptr,
		PixelFormats::uintRGBA32,
		width,
		height,
		0, // depth
		1, // mips
		0 // count
		));

	// read and copy data
	int resultPitch = textureData->GetMipLinePitch(0);
	uint8_t* resultData = (uint8_t*)textureData->GetMipData(0, 0);
	for(int i = 0; i < height; ++i)
		for(int j = 0; j < width; ++j)
		{
			for(int k = 0; k < 3; ++k)
				resultData[i * resultPitch + j * 4 + k] = pixelData[(height - i - 1) * pitch + j * 3 + 2 - k];
			resultData[i * resultPitch + j * 4 + 3] = 255; // alpha
		}

	return textureData;

	END_TRY("Can't load TGA image");
}

END_INANITY_GRAPHICS
