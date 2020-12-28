#ifndef ___INANITY_GUI_FONT_ENGINE_HPP___
#define ___INANITY_GUI_FONT_ENGINE_HPP___

#include "gui.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GUI

class FontFace;

/// Abstract class of font engine.
class FontEngine : public Object
{
public:
	virtual ptr<FontFace> LoadFontFace(ptr<File> file, int size) = 0;
};

END_INANITY_GUI

#endif
