#include "BmpImageLoader.hpp"
#include "Image2DData.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include "../windows.hpp"

ptr<Image2DData> BmpImageLoader::Load(ptr<File> file)
{
	try
	{
		// получить данные
		const unsigned char* fileData = (const unsigned char*)file->GetData();
		size_t fileSize = file->GetSize();

		if(fileSize < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
			THROW_PRIMARY_EXCEPTION("Can't read headers");

		const BITMAPFILEHEADER* bfh = (const BITMAPFILEHEADER*)fileData;
		if(bfh->bfType != 'MB')
			THROW_PRIMARY_EXCEPTION("Wrong signature");
		if(fileSize != bfh->bfSize)
			THROW_PRIMARY_EXCEPTION("Wrong file size");
		const BITMAPINFOHEADER* bih = (const BITMAPINFOHEADER*)(bfh + 1);

		int width = bih->biWidth;
		int height = bih->biHeight;
		if(height < 0)
			height = -height;
		int pitch = (width * (bih->biBitCount / 8) + 3) & ~3;
		if(bfh->bfOffBits >= fileSize || bfh->bfOffBits + height * pitch > fileSize)
			THROW_PRIMARY_EXCEPTION("Can't read pixel data");
		const unsigned char* pixelData = fileData + bfh->bfOffBits;
		int lineStep;
		const unsigned char* lineData;
		if(height < 0)
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

		int resultPitch = width * 4;
		ptr<MemoryFile> resultFile = NEW(MemoryFile(resultPitch * height));
		unsigned char* resultDataLine = (unsigned char*)resultFile->GetData();

		switch(bih->biBitCount)
		{
		case 8:
			// прочитать палитру
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
			THROW_PRIMARY_EXCEPTION("Unsupported bit count");
		}

		return NEW(Image2DData(resultFile, width, height, resultPitch, PixelFormats::intR8G8B8A8));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load BMP image", exception);
	}
}
