#include <iostream>
#include "../inanity-base.hpp"
#include "../inanity-platform.hpp"
#include "Font.hpp"
#include "FtEngine.hpp"
#include "FontFace.hpp"
#include "SwCanvas.hpp"
#include "FontGlyphs.hpp"
#include "../graphics/RawTextureData.hpp"
using namespace Inanity;
using namespace Inanity::Graphics;

void SaveToBmp(ptr<RawTextureData> rawTextureData, ptr<OutputStream> outputStream)
{
	int width = rawTextureData->GetMipWidth();
	int height = rawTextureData->GetMipHeight();
	int pitch = (width + 3) & ~3;

	BITMAPFILEHEADER bfh;
	bfh.bfType = 'MB';
	bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + pitch * height;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	outputStream->Write(&bfh, sizeof(bfh));

	BITMAPINFOHEADER bih;
	memset(&bih, 0, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = 8;
	outputStream->Write(&bih, sizeof(bih));

	RGBQUAD palette[256];
	for(int i = 0; i < 256; ++i)
	{
		palette[i].rgbBlue = i;
		palette[i].rgbGreen = i;
		palette[i].rgbRed = i;
		palette[i].rgbReserved = 0;
	}
	outputStream->Write(palette, sizeof(palette));

	const char* data = (const char*)rawTextureData->GetMipData();
	for(int i = height - 1; i >= 0; --i)
		outputStream->Write(data + i * width, pitch);
}

int main()
{
	try
	{
		ptr<FileSystem> fs = NEW(Platform::FileSystem(""));
		ptr<Gui::FontEngine> fontEngine = NEW(Gui::FtEngine());
		ptr<Gui::FontFace> fontFace = fontEngine->LoadFontFace(fs->LoadFile("gui/DejaVuSans.ttf"));
		const int fontSize = 64;
		ptr<Gui::FontShape> fontShape = fontFace->CreateShape(fontSize);
		ptr<Gui::SwCanvas> canvas = NEW(Gui::SwCanvas());
		ptr<Gui::FontGlyphs> fontGlyphs = fontFace->CreateGlyphs(canvas, fontSize);

		ptr<Gui::Font> font = NEW(Gui::Font(fontShape, fontGlyphs));

		const int destWidth = 1024;
		const int destHeight = 1024;
		const PixelFormat pixelFormat = PixelFormat(PixelFormat::pixelR, PixelFormat::formatUint, PixelFormat::size8bit);
		ptr<RawTextureData> image = NEW(RawTextureData(
			NEW(MemoryFile(destWidth * destHeight * PixelFormat::GetPixelSize(pixelFormat.size))),
			pixelFormat,
			destWidth,
			destHeight,
			0, // depth
			1, // mips
			0 // count
			));
		memset(image->GetMipData(), 128, image->GetImageSize());

		canvas->SetDestination(image);
		font->DrawString(canvas, "Inanity", vec2(100, 500));

		SaveToBmp(image, fs->SaveStream("gui/out.bmp"));
	}
	catch(Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
