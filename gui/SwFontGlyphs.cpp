#include "SwFontGlyphs.hpp"
#include "../graphics/RawTextureData.hpp"

BEGIN_INANITY_GUI

SwFontGlyphs::SwFontGlyphs(
	const GlyphInfos& glyphInfos,
	float originalFontSize,
	FontImageType fontImageType,
	float smoothCoef,
	ptr<Graphics::RawTextureData> image
) :
	FontGlyphs(glyphInfos, originalFontSize, fontImageType, smoothCoef),
	image(image)
{}

ptr<Graphics::RawTextureData> SwFontGlyphs::GetImage() const
{
	return image;
}

END_INANITY_GUI
