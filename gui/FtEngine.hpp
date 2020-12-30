#ifndef ___INANITY_GUI_FT_ENGINE_HPP___
#define ___INANITY_GUI_FT_ENGINE_HPP___

#include "FontEngine.hpp"
#include "ft.hpp"

BEGIN_INANITY_GUI

/// FreeType glyph rasterization engine.
class FtEngine : public FontEngine
{
private:
	FT_Library library;

public:
	FtEngine();
	~FtEngine();

	FT_Library GetLibrary() const;

	//*** FontEngine's methods.
	ptr<FontFace> LoadFontFace(ptr<File> file, int size);
};

END_INANITY_GUI

#endif
