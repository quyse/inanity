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

const int FtFontFace::distanceFieldScale = 32;
const int FtFontFace::distanceFieldSpread = 128;

FtFontFace::FtFontFace(ptr<FtEngine> engine, FT_Face ftFace, ptr<File> file)
: engine(engine), ftFace(ftFace), file(file) {}

FtFontFace::~FtFontFace()
{
	FT_Done_Face(ftFace);
}

ptr<FontShape> FtFontFace::CreateShape(int size)
{
	BEGIN_TRY();

	hb_font_t* hbFont = hb_ft_font_create(ftFace, nullptr);
	return NEW(HbFontShape((float)size, this, hbFont, ftFace));

	END_TRY("Can't create shape for Freetype font face");
}

ptr<FontGlyphs> FtFontFace::CreateGlyphs(Canvas* canvas, int size, FontImageType fontImageType)
{
	BEGIN_TRY();

	FT_Int32 loadFlags;
	FT_Render_Mode renderMode;
	int scale;
	switch(fontImageType)
	{
	case FontImageTypes::grayscale:
		loadFlags = FT_LOAD_NO_HINTING | FT_LOAD_TARGET_NORMAL;
		renderMode = FT_RENDER_MODE_NORMAL;
		scale = 1;
		break;
	case FontImageTypes::distanceField:
		loadFlags = FT_LOAD_NO_HINTING | FT_LOAD_TARGET_MONO;
		renderMode = FT_RENDER_MODE_MONO;
		scale = distanceFieldScale;
		break;
	default:
		THROW("Invalid font image type");
	}

	FT_Long glyphsCount = ftFace->num_glyphs;

	if(FT_Set_Pixel_Sizes(ftFace, size * scale, size * scale))
		THROW("Can't set pixel sizes");

	std::vector<ptr<RawTextureData> > glyphImages(glyphsCount);
	FontGlyphs::GlyphInfos glyphInfos(glyphsCount);

	for(FT_Long i = 0; i < glyphsCount; ++i)
	{
		if(FT_Load_Glyph(ftFace, i, loadFlags))
			THROW("Can't load glyph");

		if(FT_Render_Glyph(ftFace->glyph, renderMode))
			THROW("Can't render glyph");

		const FT_Bitmap& bitmap = ftFace->glyph->bitmap;

		ptr<RawTextureData> glyphImage;
		switch(fontImageType)
		{
		case FontImageTypes::grayscale:
			glyphImage = StoreGrayscaleImage(bitmap);
			break;
		case FontImageTypes::distanceField:
			glyphImage = StoreDistanceFieldImage(bitmap);
			break;
		}

		glyphImages[i] = glyphImage;

		FontGlyphs::GlyphInfo& glyphInfo = glyphInfos[i];
		glyphInfo.width = glyphImage->GetImageWidth();
		glyphInfo.height = glyphImage->GetImageHeight();
		glyphInfo.offsetX = (int)(ftFace->glyph->bitmap_left / scale);
		glyphInfo.offsetY = -(int)(ftFace->glyph->bitmap_top / scale);
	}

	// unite glyph images
	std::vector<std::pair<int, int> > glyphPositions;
	ptr<RawTextureData> glyphsImage = RawTextureData::ShelfUnion(glyphImages, glyphPositions, 1024, 1, true);

	// set glyph positions
	for(FT_Long i = 0; i < glyphsCount; ++i)
	{
		glyphInfos[i].leftTopX = glyphPositions[i].first;
		glyphInfos[i].leftTopY = glyphPositions[i].second;
	}

	return canvas->CreateGlyphs(
		glyphInfos,
		(float)size,
		fontImageType,
		(float)distanceFieldSpread / (float)distanceFieldScale,
		glyphsImage
	);

	END_TRY("Can't create Freetype font instance");
}

ptr<RawTextureData> FtFontFace::StoreGrayscaleImage(const FT_Bitmap& bitmap)
{
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

	return glyphImage;
}

ptr<RawTextureData> FtFontFace::StoreDistanceFieldImage(const FT_Bitmap& bitmap)
{
	ptr<RawTextureData> glyphImage;

	if(bitmap.width > 0 && bitmap.rows > 0)
	{
		THROW_ASSERT(bitmap.pixel_mode == FT_PIXEL_MODE_MONO);

#define ROW(i) (unsigned char*)(bitmap.buffer + (bitmap.pitch >= 0 ? i : (bitmap.rows - 1 - i)) * bitmap.pitch)
#define PIXEL(row, j) (!!(row[j / 8] & (1 << (7 - (j % 8)))))

		int q = distanceFieldSpread;

		int resultWidth = (bitmap.width + q * 2 + distanceFieldScale - 1) / distanceFieldScale;
		int resultHeight = (bitmap.rows + q * 2 + distanceFieldScale - 1) / distanceFieldScale;

		ptr<File> pixelsFile = NEW(MemoryFile(resultWidth * resultHeight));
		unsigned char* pixelsData = (unsigned char*)pixelsFile->GetData();

		// loop for dest pixels
		for(int di = 0; di < resultHeight; ++di)
		{
			int si = di * distanceFieldScale - q;
			unsigned char* sRow = ROW(si);

			for(int dj = 0; dj < resultWidth; ++dj)
			{
				int sj = dj * distanceFieldScale - q;
				int sPixel = (si >= 0 && si < bitmap.rows && sj >= 0 && sj < bitmap.width) ? PIXEL(sRow, sj) : 0;

				double minDist = distanceFieldSpread;

				// loop for source pixels to determine nearest with opposite color
				int ki1 = si - distanceFieldSpread;
				int ki2 = si + distanceFieldSpread + 1;
				int kj1 = sj - distanceFieldSpread;
				int kj2 = sj + distanceFieldSpread + 1;
				for(int ki = ki1; ki < ki2; ++ki)
				{
					if(ki <= si - minDist)
						continue;
					if(ki >= si + minDist)
						break;

					unsigned char* kRow = ROW(ki);

					int ii = si - ki;
					ii *= ii;

					for(int kj = kj1; kj < kj2; ++kj)
					{
						if(kj <= sj - minDist)
							continue;
						if(kj >= sj + minDist)
							break;

						int kPixel = (ki >= 0 && ki < bitmap.rows && kj >= 0 && kj < bitmap.width) ? PIXEL(kRow, kj) : 0;

						if(sPixel != kPixel)
						{
							int jj = sj - kj;
							jj *= jj;

							minDist = std::min(minDist, sqrt((double)(ii + jj)));
						}
					}
				}

				minDist = (sPixel ? minDist : -minDist) / (double)distanceFieldSpread;
				minDist = (minDist + 1.0f) * 0.5f;

				int pixelValue = std::min(std::max((int)(minDist * 255), 0), 255);

				pixelsData[di * resultWidth + dj] = (unsigned char)pixelValue;
			}
		}

#undef ROW
#undef PIXEL

		glyphImage = NEW(RawTextureData(
			pixelsFile,
			PixelFormat(
				PixelFormat::pixelR,
				PixelFormat::formatUint,
				PixelFormat::size8bit),
			resultWidth, // width
			resultHeight, // height
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

	return glyphImage;
}

END_INANITY_GUI
