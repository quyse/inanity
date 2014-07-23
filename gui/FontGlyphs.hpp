#ifndef ___INANITY_GUI_FONT_GLYPHS_HPP___
#define ___INANITY_GUI_FONT_GLYPHS_HPP___

#include "FontImageType.hpp"
#include <vector>

BEGIN_INANITY_GUI

/// Abstract font glyphs class.
/** This class depends on Canvas. */
class FontGlyphs : public Object
{
public:
	/// Glyph info (for creating glyphs).
	/** All coordinates in pixels, (0, 0) is top-left. */
	struct GlyphInfo
	{
		/// Size of glyph on texture.
		int width, height;
		/// Coordinates of left-top corner of the glyph on texture.
		int leftTopX, leftTopY;
		/// Offset from pen point to left-top corner on canvas.
		int offsetX, offsetY;
	};

	typedef std::vector<GlyphInfo> GlyphInfos;

protected:
	GlyphInfos glyphInfos;
	/// Size of font used to create glyphs.
	float originalFontSize;
	/// Type of font image.
	FontImageType fontImageType;
	/// Smooth coefficient for distance field.
	float smoothCoef;

	FontGlyphs(const GlyphInfos& glyphInfos, float originalFontSize, FontImageType fontImageType, float smoothCoef);

public:
	const GlyphInfos& GetGlyphInfos() const;
	float GetOriginalFontSize() const;
	FontImageType GetFontImageType() const;
	float GetSmoothCoef() const;
};

END_INANITY_GUI

#endif
