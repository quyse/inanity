#include "GrFontGlyphs.hpp"
#include "../graphics/Texture.hpp"

BEGIN_INANITY_GUI

GrFontGlyphs::GrFontGlyphs(
	const GlyphInfos& glyphInfos,
	float originalFontSize,
	FontImageType fontImageType,
	float smoothCoef,
	ptr<Graphics::Texture> texture,
	const Glyphs& glyphs
) :
	FontGlyphs(glyphInfos, originalFontSize, fontImageType, smoothCoef),
	texture(texture),
	glyphs(glyphs)
{}

ptr<Graphics::Texture> GrFontGlyphs::GetTexture() const
{
	return texture;
}

const GrFontGlyphs::Glyphs& GrFontGlyphs::GetGlyphs() const
{
	return glyphs;
}

END_INANITY_GUI
