#include "FtEngine.hpp"
#include "FtFontFace.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GUI

FtEngine::FtEngine()
{
	if(FT_Init_FreeType(&library))
		THROW("Can't init FreeType");
}

FtEngine::~FtEngine()
{
	FT_Done_FreeType(library);
}

FT_Library FtEngine::GetLibrary() const
{
	return library;
}

ptr<FontFace> FtEngine::LoadFontFace(ptr<File> file, int size)
{
	BEGIN_TRY();

	return NEW(FtFontFace(this, file, size));

	END_TRY("Can't load Freetype font");
}

END_INANITY_GUI
