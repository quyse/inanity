#include "SwFontGlyphs.hpp"
#include "../graphics/RawTextureData.hpp"

BEGIN_INANITY_GUI

SwFontGlyphs::SwFontGlyphs(ptr<Graphics::RawTextureData> image, const GlyphInfos& glyphInfos)
: image(image), glyphInfos(glyphInfos) {}

ptr<Graphics::RawTextureData> SwFontGlyphs::GetImage() const
{
	return image;
}

const SwFontGlyphs::GlyphInfos& SwFontGlyphs::GetGlyphInfos() const
{
	return glyphInfos;
}

END_INANITY_GUI
