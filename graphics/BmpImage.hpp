#ifndef ___INANITY_GRAPHICS_BMP_IMAGE_HPP___
#define ___INANITY_GRAPHICS_BMP_IMAGE_HPP___

#include "graphics.hpp"
#include <cstdint>

BEGIN_INANITY

class File;
class OutputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

class RawTextureData;
class ImageLoader;

/// Utility functions for images in BMP format.
class BmpImage : public Object
{
private:
#pragma pack(push, 1)
	struct Header
	{
		// BITMAPFILEHEADER
		static const uint8_t magicValue[2];
		uint8_t magic[2];
		uint32_t size;
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t dataOffset;
		// BITMAPINFOHEADER
		static const uint32_t header2SizeValue = 40;
		uint32_t header2Size;
		int32_t width;
		int32_t height;
		uint16_t planesCount;
		uint16_t bitCount;
		uint32_t compression;
		uint32_t sizeImage;
		int32_t dpiX;
		int32_t dpiY;
		uint32_t usedColorsCount;
		uint32_t importantColorsCount;
	};
	struct PaletteEntry
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t reserved; // should be zero
	};
#pragma pack(pop)

	class Loader;

public:
	static ptr<RawTextureData> Load(ptr<File> file);
	static void Save(ptr<RawTextureData> rawTextureData, ptr<OutputStream> outputStream);

	static ptr<ImageLoader> CreateLoader();
};

END_INANITY_GRAPHICS

#endif
