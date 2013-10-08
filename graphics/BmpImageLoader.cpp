#include "BmpImageLoader.hpp"
#include "RawTextureData.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

#ifdef ___INANITY_WINDOWS
#include "../platform/windows.hpp"
#endif

BEGIN_INANITY_GRAPHICS

ptr<RawTextureData> BmpImageLoader::Load(ptr<File> file)
{
#ifdef ___INANITY_WINDOWS
	try
	{
		const unsigned char* fileData = (const unsigned char*)file->GetData();
		size_t fileSize = file->GetSize();

		if(fileSize < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
			THROW("Can't read headers");

		const BITMAPFILEHEADER* bfh = (const BITMAPFILEHEADER*)fileData;
		if(bfh->bfType != 'MB')
			THROW("Wrong signature");
		if(fileSize != bfh->bfSize)
			THROW("Wrong file size");
		const BITMAPINFOHEADER* bih = (const BITMAPINFOHEADER*)(bfh + 1);

		int width = bih->biWidth;
		int height = bih->biHeight;
		if(height < 0)
			height = -height;
		int pitch = (width * (bih->biBitCount / 8) + 3) & ~3;
		if(bfh->bfOffBits >= fileSize || bfh->bfOffBits + height * pitch > fileSize)
			THROW("Can't read pixel data");
		const unsigned char* pixelData = fileData + bfh->bfOffBits;
		int lineStep;
		const unsigned char* lineData;
		if(bih->biHeight < 0)
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

		switch(bih->biBitCount)
		{
		case 8:
			{
				const RGBQUAD* palette = (const RGBQUAD*)(bih + 1);
				for(int i = 0; i < height; ++i)
				{
					for(int j = 0; j < width; ++j)
					{
						const RGBQUAD& p = palette[lineData[j]];
						resultDataLine[j * 4 + 0] = p.rgbRed;
						resultDataLine[j * 4 + 1] = p.rgbGreen;
						resultDataLine[j * 4 + 2] = p.rgbBlue;
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
						resultDataLine[j * 4 + k] = lineData[j * 3 + k];
					resultDataLine[j * 4 + 3] = 255;
				}
				lineData += lineStep;
				resultDataLine += resultPitch;
			}
			break;
		case 32:
			for(int i = 0; i < height; ++i)
			{
				memcpy(resultDataLine, lineData, resultPitch);
				lineData += lineStep;
				resultDataLine += resultPitch;
			}
			break;
		default:
			THROW("Unsupported bit count");
		}

		return textureData;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't load BMP image", exception);
	}
#else // ___INANITY_WINDOWS
	THROW("BmpImageLoader implemented only on Windows");
#endif
}

END_INANITY_GRAPHICS
