#ifndef ___INANITY_GUI_GR_CANVAS_HPP___
#define ___INANITY_GUI_GR_CANVAS_HPP___

#include "Canvas.hpp"
#include "../graphics/graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Device;
class Context;
class Texture;
class ShaderCache;

END_INANITY_GRAPHICS

BEGIN_INANITY_GUI

class GrCanvas : public Canvas
{
public:
	/// Subpixel modes.
	enum SubPixelMode
	{
		subPixelModeOff,

		subPixelModeOnBegin,

		subPixelModeHorizontalRGB = subPixelModeOnBegin,
		subPixelModeHorizontalBGR,
		subPixelModeVerticalRGB,
		subPixelModeVerticalBGR,

		subPixelModeOnEnd,

		subPixelModesCount = subPixelModeOnEnd
	};

private:
	ptr<Graphics::Device> device;

	struct Helper;
	ptr<Helper> helper;

	/// Subpixel mode.
	SubPixelMode subPixelMode;

	/// Current graphics context.
	ptr<Graphics::Context> context;
	/// Number of chars queued.
	int queuedGlyphsCount;
	/// Last set font texture.
	ptr<Graphics::Texture> currentFontTexture;

	GrCanvas(ptr<Graphics::Device> device, ptr<Helper> helper);

public:
	void SetContext(ptr<Graphics::Context> context);

	static ptr<GrCanvas> Create(ptr<Graphics::Device> device, ptr<Graphics::ShaderCache> shaderCache);

	void SetSubPixelMode(SubPixelMode subPixelMode);

	//*** Canvas' methods.
	ptr<FontGlyphs> CreateGlyphs(
		ptr<Graphics::RawTextureData> image,
		const FontGlyphs::GlyphInfos& glyphInfos,
		int scaleX,
		int scaleY
	);
	void DrawGlyph(FontGlyphs* glyphs, int glyphIndex, const Graphics::vec2& penPoint, const Graphics::vec4& color);
	void Flush();
};

END_INANITY_GUI

#endif
