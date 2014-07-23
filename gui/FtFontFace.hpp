#ifndef ___INANITY_GUI_FT_FONT_FACE_HPP___
#define ___INANITY_GUI_FT_FONT_FACE_HPP___

#include "FontFace.hpp"
#include "ft.hpp"
#include "../graphics/graphics.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class RawTextureData;

END_INANITY_GRAPHICS

BEGIN_INANITY_GUI

class FtEngine;
class Canvas;

class FtFontFace : public FontFace
{
private:
	ptr<FtEngine> engine;
	FT_Face ftFace;
	/// File from which face was created.
	/** File memory should not be freed
	until FT_Face is live. */
	ptr<File> file;

	/// Size multiplier for distance fields generation.
	static const int distanceFieldScale;
	/// Spread for searching nearest pixel of opposite color.
	static const int distanceFieldSpread;

	ptr<Graphics::RawTextureData> StoreGrayscaleImage(const FT_Bitmap& bitmap);
	ptr<Graphics::RawTextureData> StoreDistanceFieldImage(const FT_Bitmap& bitmap);

public:
	FtFontFace(ptr<FtEngine> engine, FT_Face ftFace, ptr<File> file);
	~FtFontFace();

	//*** FontFace's methods.
	ptr<FontShape> CreateShape(int size);
	ptr<FontGlyphs> CreateGlyphs(Canvas* canvas, int size, FontImageType fontImageType);
};

END_INANITY_GUI

#endif
