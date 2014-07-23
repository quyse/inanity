#include <iostream>
#include "../inanity-base.hpp"
#include "../inanity-platform.hpp"
#include "Font.hpp"
#include "FtEngine.hpp"
#include "FontFace.hpp"
#include "SwCanvas.hpp"
#include "SwFontGlyphs.hpp"
#include "FontGlyphs.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../graphics/BmpImage.hpp"
using namespace Inanity;
using namespace Inanity::Graphics;

int main()
{
	try
	{
		ptr<FileSystem> fs = NEW(Platform::FileSystem(""));
		ptr<Gui::FontEngine> fontEngine = NEW(Gui::FtEngine());
		ptr<Gui::FontFace> fontFace = fontEngine->LoadFontFace(fs->LoadFile("/gui/DejaVuSans.ttf"));
		ptr<Gui::FontShape> fontShape = fontFace->CreateShape(30);
		ptr<Gui::SwCanvas> canvas = NEW(Gui::SwCanvas());
		ptr<Gui::FontGlyphs> fontGlyphs = fontFace->CreateGlyphs(canvas, 30, Gui::FontImageTypes::distanceField);

		BmpImage::Save(fontGlyphs.FastCast<Gui::SwFontGlyphs>()->GetImage(), fs->SaveStream("/gui/alphabat.bmp"));

		ptr<Gui::Font> font = NEW(Gui::Font(fontShape, fontGlyphs));

		const int destWidth = 1024;
		const int destHeight = 1024;
		const PixelFormat pixelFormat = PixelFormat(PixelFormat::pixelR, PixelFormat::formatUint, PixelFormat::size24bit);
		ptr<RawTextureData> image = NEW(RawTextureData(
			NEW(MemoryFile(destWidth * destHeight * PixelFormat::GetPixelSize(pixelFormat.size))),
			pixelFormat,
			destWidth,
			destHeight,
			0, // depth
			1, // mips
			0 // count
			));
		memset(image->GetMipData(), 255, image->GetImageSize());

		canvas->SetDestination(image);
		font->DrawString(canvas, "Inanity", vec2(100.0f, 500.0f), vec4(0, 0, 0, 1));

		BmpImage::Save(image, fs->SaveStream("/gui/out.bmp"));
	}
	catch(Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
