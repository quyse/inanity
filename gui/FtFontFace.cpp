#include "FtFontFace.hpp"
#include "FtEngine.hpp"
#include "FontGlyphs.hpp"
#include "Canvas.hpp"
#include "HbFontShape.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include FT_TRUETYPE_TABLES_H

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

ptr<FontGlyphs> FtFontFace::CreateGlyphs(Canvas* canvas, int size, const CreateGlyphsConfig& config)
{
	BEGIN_TRY();

	int halfScaleX = config.halfScaleX;
	int halfScaleY = config.halfScaleY;
	std::vector<int>* glyphsNeeded = config.glyphsNeeded;

	FT_Long glyphsCount = glyphsNeeded ? glyphsNeeded->size() : ftFace->num_glyphs;

	if(FT_Set_Pixel_Sizes(ftFace, size * (halfScaleX * 2 + 1), size * (halfScaleY * 2 + 1)))
		THROW("Can't set pixel sizes");

	std::vector<ptr<RawTextureData> > glyphImages(glyphsCount);
	FontGlyphs::GlyphInfos glyphInfos(glyphsCount);

	for(FT_Long i = 0; i < glyphsCount; ++i)
	{
		if(FT_Load_Glyph(ftFace, glyphsNeeded ? (*glyphsNeeded)[i] : i, config.enableHinting ? FT_LOAD_DEFAULT : FT_LOAD_NO_HINTING))
			THROW("Can't load glyph");

		if(FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL))
			THROW("Can't render glyph");

		const FT_Bitmap& bitmap = ftFace->glyph->bitmap;

		ptr<RawTextureData> glyphImage;
		if(bitmap.width > 0 && bitmap.rows > 0)
		{
			THROW_ASSERT(bitmap.pixel_mode == FT_PIXEL_MODE_GRAY);

			ptr<File> sourcePixelsFile = NEW(MemoryFile(bitmap.width * bitmap.rows));
			unsigned char* sourcePixelsData = (unsigned char*)sourcePixelsFile->GetData();
			for(int i = 0; i < (int)bitmap.rows; ++i)
				memcpy(
					sourcePixelsData + i * bitmap.width,
					bitmap.buffer + (bitmap.pitch >= 0 ? i : (bitmap.rows - 1 - i)) * bitmap.pitch,
					bitmap.width);

			int pixelsWidth = bitmap.width + halfScaleX * 2;
			int pixelsHeight = bitmap.rows + halfScaleY * 2;

			ptr<File> pixelsFile;
			if(halfScaleX || halfScaleY)
			{
				pixelsFile = NEW(MemoryFile(pixelsWidth * pixelsHeight));
				unsigned char* pixelsData = (unsigned char*)pixelsFile->GetData();
				int fullScale = (halfScaleX * 2 + 1) * (halfScaleY * 2 + 1);
				for(int i = 0; i < pixelsHeight; ++i)
					for(int j = 0; j < pixelsWidth; ++j)
					{
						int mini = std::max(i - halfScaleY * 2, 0);
						int maxi = std::min(i + 1, (int)bitmap.rows);
						int minj = std::max(j - halfScaleX * 2, 0);
						int maxj = std::min(j + 1, (int)bitmap.width);
						int s = 0;
						for(int ii = mini; ii < maxi; ++ii)
							for(int jj = minj; jj < maxj; ++jj)
								s += sourcePixelsData[ii * bitmap.width + jj];
						pixelsData[i * pixelsWidth + j] = (unsigned char)(s / fullScale);
					}

				sourcePixelsFile = nullptr;
			}
			else
				pixelsFile = sourcePixelsFile;

			glyphImage = NEW(RawTextureData(
				pixelsFile,
				PixelFormat(
					PixelFormat::pixelR,
					PixelFormat::formatUint,
					PixelFormat::size8bit),
				pixelsWidth, // width
				pixelsHeight, // height
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
		glyphInfo.offsetX = (int)ftFace->glyph->bitmap_left + halfScaleX;
		glyphInfo.offsetY = -(int)ftFace->glyph->bitmap_top + halfScaleY;
	}

	// unite glyph images
	std::vector<std::pair<int, int> > glyphPositions;
	ptr<RawTextureData> glyphsImage = RawTextureData::ShelfUnion(glyphImages, glyphPositions, config.maxTextureWidth, 1, true);

	if(glyphsImage->GetImageHeight() > config.maxTextureHeight) THROW("Result texture is too big");

	// set glyph positions
	for(FT_Long i = 0; i < glyphsCount; ++i)
	{
		glyphInfos[i].leftTopX = glyphPositions[i].first;
		glyphInfos[i].leftTopY = glyphPositions[i].second;
	}

	return canvas->CreateGlyphs(glyphsImage, glyphInfos, 1 + halfScaleX * 2, 1 + halfScaleY * 2);

	END_TRY("Can't create Freetype font instance");
}

FontFace::Metrics FtFontFace::CalculateMetrics(int size) const
{
	float scale = (float)size / (float)ftFace->units_per_EM;
	Metrics metrics;
	metrics.ascender = (float)ftFace->ascender * scale;
	metrics.descender = (float)ftFace->descender * scale;
	metrics.height = (float)ftFace->height * scale;

	{
		TT_PCLT* pcltTable;
		TT_OS2* os2Table;
		if((pcltTable = (TT_PCLT*)FT_Get_Sfnt_Table(ftFace, FT_SFNT_PCLT)) != nullptr)
			metrics.capHeight = (float)pcltTable->CapHeight * scale;
		else if((os2Table = (TT_OS2*)FT_Get_Sfnt_Table(ftFace, FT_SFNT_OS2)) != nullptr && os2Table->version != 0xFFFF && os2Table->version >= 2)
			metrics.capHeight = (float)os2Table->sCapHeight * scale;
	}

	return metrics;
}

END_INANITY_GUI
