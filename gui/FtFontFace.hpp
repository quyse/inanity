#ifndef ___INANITY_GUI_FT_FONT_FACE_HPP___
#define ___INANITY_GUI_FT_FONT_FACE_HPP___

#include "FontFace.hpp"
#include "ft.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GUI

class FtEngine;
class Canvas;

class FtFontFace : public FontFace
{
private:
	ptr<FtEngine> engine;
	/// File from which face was created.
	/** File memory should not be freed
	until FT_Face is live. */
	ptr<File> file;
	int size;
	// Main face.
	FT_Face ftFace = nullptr;
	// Separate immutable face for shapes.
	FT_Face ftShapeFace = nullptr;

public:
	FtFontFace(ptr<FtEngine> engine, ptr<File> file, int size);
	~FtFontFace();

	//*** FontFace's methods.
	ptr<FontShape> CreateShape();
	ptr<FontGlyphs> CreateGlyphs(Canvas* canvas, const CreateGlyphsConfig& config);
	Metrics CalculateMetrics() const;
};

END_INANITY_GUI

#endif
