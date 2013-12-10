#include "FtFontFace.hpp"
#include "FtEngine.hpp"
#include "FontGlyphs.hpp"
#include "Canvas.hpp"
#include "HbFontShape.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GUI

using namespace Graphics;

FtFontFace::FtFontFace(ptr<FtEngine> engine, FT_Face ftFace, ptr<File> file)
: engine(engine), ftFace(ftFace), file(file) {}

FtFontFace::~FtFontFace()
{
	FT_Done_Face(ftFace);
}

ptr<FontShape> FtFontFace::CreateShape(int size)
{
	BEGIN_TRY();

	if(FT_Set_Pixel_Sizes(ftFace, size, size))
		THROW("Can't set pixel sizes");

	hb_font_t* hbFont = hb_ft_font_create(ftFace, nullptr);
	return NEW(HbFontShape(this, hbFont));

	END_TRY("Can't create shape for Freetype font face");
}

ptr<FontGlyphs> FtFontFace::CreateGlyphs(Canvas* canvas, int size)
{
	BEGIN_TRY();

	FT_Long glyphsCount = ftFace->num_glyphs;

	if(FT_Set_Pixel_Sizes(ftFace, size, size))
		THROW("Can't set pixel sizes");

	std::vector<ptr<RawTextureData> > glyphImages(glyphsCount);
	FontGlyphs::GlyphInfos glyphInfos(glyphsCount);

	for(FT_Long i = 0; i < glyphsCount; ++i)
	{
		if(FT_Load_Glyph(ftFace, i, 0))
			THROW("Can't load glyph");

		if(FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL))
			THROW("Can't render glyph");

		const FT_Bitmap& bitmap = ftFace->glyph->bitmap;

		ptr<RawTextureData> glyphImage;
		if(bitmap.width > 0 && bitmap.rows > 0)
		{
			THROW_ASSERT(bitmap.pixel_mode == FT_PIXEL_MODE_GRAY);

			ptr<File> pixelsFile = NEW(MemoryFile(bitmap.width * bitmap.rows));
			char* pixelsData = (char*)pixelsFile->GetData();
			for(int i = 0; i < bitmap.rows; ++i)
				memcpy(
					pixelsData + i * bitmap.width,
					bitmap.buffer + (bitmap.pitch >= 0 ? i : (bitmap.rows - 1 - i)) * bitmap.pitch,
					bitmap.width);

			glyphImage = NEW(RawTextureData(
				pixelsFile,
				PixelFormat(
					PixelFormat::pixelR,
					PixelFormat::formatUint,
					PixelFormat::size8bit),
				bitmap.width, // width
				bitmap.rows, // height
				0, // depth
				1, // mips
				0 // count
				));
		}
		else
		{
			// empty glyph
			ptr<File> pixelsFile = NEW(MemoryFile(1));
			*(unsigned char*)pixelsFile->GetData() = 0;
			glyphImage = NEW(RawTextureData(
				pixelsFile,
				PixelFormat(
					PixelFormat::pixelR,
					PixelFormat::formatUint,
					PixelFormat::size8bit),
				1, // width
				1, // height
				0, // depth
				1, // mips
				0 // count
				));
		}

		glyphImages[i] = glyphImage;

		FontGlyphs::GlyphInfo& glyphInfo = glyphInfos[i];
		glyphInfo.width = glyphImage->GetImageWidth();
		glyphInfo.height = glyphImage->GetImageHeight();
		glyphInfo.offsetX = (int)ftFace->glyph->bitmap_left;
		glyphInfo.offsetY = -(int)ftFace->glyph->bitmap_top;
	}

	// unite glyph images
	std::vector<std::pair<int, int> > glyphPositions;
	ptr<RawTextureData> glyphsImage = RawTextureData::ShelfUnion(glyphImages, 1024, 1, glyphPositions);

	// set glyph positions
	for(FT_Long i = 0; i < glyphsCount; ++i)
	{
		glyphInfos[i].leftTopX = glyphPositions[i].first;
		glyphInfos[i].leftTopY = glyphPositions[i].second;
	}

	return canvas->CreateGlyphs(glyphsImage, glyphInfos);

	END_TRY("Can't create Freetype font instance");
}

END_INANITY_GUI
