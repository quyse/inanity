#include "BmpImage.hpp"
#include "RawTextureData.hpp"
#include "ImageLoader.hpp"
#include "../MemoryFile.hpp"
#include "../OutputStream.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

const uint8_t BmpImage::Header::magicValue[2] = { 'B', 'M' };

class BmpImage::Loader : public ImageLoader
{
public:
	ptr<RawTextureData> Load(ptr<File> file)
	{
		return BmpImage::Load(file);
	}
};

ptr<RawTextureData> BmpImage::Load(ptr<File> file)
{
	BEGIN_TRY();

	const unsigned char* fileData = (const unsigned char*)file->GetData();
	size_t fileSize = file->GetSize();

	if(fileSize < sizeof(Header))
		THROW("Can't read header");

	const Header* header = (const Header*)fileData;
	if(memcmp(header->magic, Header::magicValue, sizeof(header->magic)) != 0)
		THROW("Wrong magic");
	if(fileSize != header->size)
		THROW("Wrong file size");

	int width = header->width;
	int height = header->height;
	if(height < 0)
		height = -height;
	int pitch = (width * (header->bitCount / 8) + 3) & ~3;
	if(header->dataOffset >= fileSize || header->dataOffset + height * pitch > fileSize)
		THROW("Can't read pixel data");
	const unsigned char* pixelData = fileData + header->dataOffset;
	int lineStep;
	const unsigned char* lineData;
	if(header->height < 0)
	{
		// top-to-bottom format
		lineStep = pitch;
		lineData = pixelData;
	}
	else
	{
		// bottom-to-top format
		lineStep = -pitch;
		lineData = pixelData + pitch * (height - 1);
	}

	ptr<RawTextureData> textureData = NEW(RawTextureData(0, PixelFormats::uintRGBA32, width, height, 0, 1, 0));

	int resultPitch = textureData->GetMipLinePitch(0);
	unsigned char* resultDataLine = (unsigned char*)textureData->GetMipData(0, 0);

	switch(header->bitCount)
	{
	case 8:
		{
			const PaletteEntry* palette = (const PaletteEntry*)(header + 1);
			for(int i = 0; i < height; ++i)
			{
				for(int j = 0; j < width; ++j)
				{
					const PaletteEntry& p = palette[lineData[j]];
					resultDataLine[j * 4 + 0] = p.red;
					resultDataLine[j * 4 + 1] = p.green;
					resultDataLine[j * 4 + 2] = p.blue;
					resultDataLine[j * 4 + 3] = 255;
				}
				lineData += lineStep;
				resultDataLine += resultPitch;
			}
		}
		break;
	case 24:
		for(int i = 0; i < height; ++i)
		{
			for(int j = 0; j < width; ++j)
			{
				for(int k = 0; k < 3; ++k)
					resultDataLine[j * 4 + k] = lineData[j * 3 + 2 - k];
				resultDataLine[j * 4 + 3] = 255;
			}
			lineData += lineStep;
			resultDataLine += resultPitch;
		}
		break;
	case 32:
		for(int i = 0; i < height; ++i)
		{
			for(int j = 0; j < width; ++j)
			{
				for(int k = 0; k < 3; ++k)
					resultDataLine[j * 4 + k] = lineData[j * 4 + 2 - k];
				resultDataLine[j * 4 + 3] = lineData[j * 4 + 3];
			}
			lineData += lineStep;
			resultDataLine += resultPitch;
		}
		break;
	default:
		THROW("Unsupported bit count");
	}

	return textureData;

	END_TRY("Can't load BMP image");
}

void BmpImage::Save(ptr<RawTextureData> rawTextureData, ptr<OutputStream> outputStream)
{
	BEGIN_TRY();

	int width = rawTextureData->GetMipWidth();
	int height = rawTextureData->GetMipHeight();
	int sourcePitch = rawTextureData->GetMipLinePitch();
	int pixelSize = rawTextureData->GetPixelSize();
	int pitch = (width * pixelSize + 3) & ~3;

	Header header;
	memset(&header, 0, sizeof(header));
	memcpy(header.magic, Header::magicValue, sizeof(header.magic));
	header.size = sizeof(Header) + (pixelSize == 1 ? 256 * sizeof(PaletteEntry) : 0) + pitch * height;
	header.dataOffset = sizeof(Header) + (pixelSize == 1 ? 256 * sizeof(PaletteEntry) : 0);
	header.header2Size = Header::header2SizeValue;
	header.width = width;
	header.height = height;
	header.planesCount = 1;
	header.bitCount = pixelSize * 8;

	outputStream->Write(&header, sizeof(header));

	if(pixelSize == 1)
	{
		PaletteEntry palette[256];
		for(int i = 0; i < 256; ++i)
		{
			palette[i].blue = i;
			palette[i].green = i;
			palette[i].red = i;
			palette[i].reserved = 0;
		}
		outputStream->Write(palette, sizeof(palette));
	}

	const char* sourceLine = (const char*)rawTextureData->GetMipData() + (height - 1) * sourcePitch;

	switch(pixelSize)
	{
	case 1:
		for(int i = 0; i < height; ++i)
		{
			outputStream->Write(sourceLine, pitch);
			sourceLine -= sourcePitch;
		}
		break;
	case 3:
		{
			ptr<MemoryFile> destLineFile = NEW(MemoryFile(pitch));
			char* destLine = (char*)destLineFile->GetData();

			for(int i = 0; i < height; ++i)
			{
				for(int j = 0; j < width; ++j)
					for(int k = 0; k < 3; ++k)
						destLine[j * 3 + k] = sourceLine[j * 3 + 2 - k];
				outputStream->Write(destLine, pitch);
				sourceLine -= sourcePitch;
			}
		}
		break;
	case 4:
		{
			ptr<MemoryFile> destLineFile = NEW(MemoryFile(pitch));
			char* destLine = (char*)destLineFile->GetData();

			for(int i = 0; i < height; ++i)
			{
				for(int j = 0; j < width; ++j)
				{
					for(int k = 0; k < 3; ++k)
						destLine[j * 4 + k] = sourceLine[j * 4 + 2 - k];
					destLine[j * 4 + 3] = sourceLine[j * 4 + 3];
				}
				outputStream->Write(destLine, pitch);
				sourceLine -= sourcePitch;
			}
		}
		break;
	default:
		THROW("Unsupported pixel size");
	}

	END_TRY("Can't save BMP image");
}

ptr<ImageLoader> BmpImage::CreateLoader()
{
	return NEW(Loader());
}

END_INANITY_GRAPHICS
