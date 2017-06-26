#ifndef ___INANITY_GUI_FONT_FACE_HPP___
#define ___INANITY_GUI_FONT_FACE_HPP___

#include "gui.hpp"
#include <vector>

BEGIN_INANITY_GUI

class FontShape;
class Canvas;
class FontGlyphs;

/// Abstract font face class.
class FontFace : public Object
{
public:
	struct CreateGlyphsConfig
	{
		int halfScaleX;
		int halfScaleY;
		std::vector<int>* glyphsNeeded;
		int maxTextureWidth;
		int maxTextureHeight;

		CreateGlyphsConfig()
		: halfScaleX(0), halfScaleY(0), glyphsNeeded(nullptr), maxTextureWidth(4096), maxTextureHeight(4096) {}
	};

public:
	/// Create font shape for the given font size.
	virtual ptr<FontShape> CreateShape(int size) = 0;
	/// Create font glyphs for the given font size.
	/** Size in pixels. Scale is for better quality.
	Half scale means additional number of pixels added for either side.
	i.e. half scale = 2 means x5, = 0 means x1. */
	virtual ptr<FontGlyphs> CreateGlyphs(Canvas* canvas, int size, const CreateGlyphsConfig& config) = 0;
};

END_INANITY_GUI

#endif
