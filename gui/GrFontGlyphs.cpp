#include "GrFontGlyphs.hpp"
#include "../graphics/Texture.hpp"

BEGIN_INANITY_GUI

GrFontGlyphs::GrFontGlyphs(ptr<Graphics::Texture> texture, const Glyphs& glyphs)
: texture(texture), glyphs(glyphs) {}

ptr<Graphics::Texture> GrFontGlyphs::GetTexture() const
{
	return texture;
}

const GrFontGlyphs::Glyphs& GrFontGlyphs::GetGlyphs() const
{
	return glyphs;
}

END_INANITY_GUI
