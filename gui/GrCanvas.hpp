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
private:
	ptr<Graphics::Device> device;

	struct Helper;
	ptr<Helper> helper;

	/// Current graphics context.
	ptr<Graphics::Context> context;
	/// Number of chars queued.
	int queuedGlyphsCount;
	/// Last set font texture.
	ptr<Graphics::Texture> currentFontTexture;
	/// Last set smooth coef.
	float currentSmoothCoef;
	/// Is current mode is distance field.
	bool currentDistanceFieldMode;

	GrCanvas(ptr<Graphics::Device> device, ptr<Helper> helper);

public:
	void SetContext(ptr<Graphics::Context> context);

	static ptr<GrCanvas> Create(ptr<Graphics::Device> device, ptr<Graphics::ShaderCache> shaderCache);

	//*** Canvas' methods.
	ptr<FontGlyphs> CreateGlyphs(
		const FontGlyphs::GlyphInfos& glyphInfos,
		float originalFontSize,
		FontImageType fontImageType,
		float smoothCoef,
		ptr<Graphics::RawTextureData> image
	);
	void DrawGlyph(
		FontGlyphs* glyphs,
		int glyphIndex,
		const Graphics::vec2& penPoint,
		float scale,
		const Graphics::vec4& color
	);
	void Flush();
};

END_INANITY_GUI

#endif
