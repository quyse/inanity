#include "SwFontGlyphs.hpp"
#include "../graphics/RawTextureData.hpp"

BEGIN_INANITY_GUI

SwFontGlyphs::SwFontGlyphs(const GlyphInfos& glyphInfos, int scaleX, int scaleY, ptr<Graphics::RawTextureData> image)
: FontGlyphs(glyphInfos, scaleX, scaleY), image(image) {}

ptr<Graphics::RawTextureData> SwFontGlyphs::GetImage() const
{
	return image;
}

END_INANITY_GUI
