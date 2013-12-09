#include "SwCanvas.hpp"
#include "SwFontGlyphs.hpp"
#include "../graphics/RawTextureData.hpp"

BEGIN_INANITY_GUI

void SwCanvas::SetDestination(ptr<Graphics::RawTextureData> destination)
{
	this->destination = destination;
}

ptr<FontGlyphs> SwCanvas::CreateGlyphs(
	ptr<Graphics::RawTextureData> image,
	const FontGlyphs::GlyphInfos& glyphInfos
)
{
	return NEW(SwFontGlyphs(image, glyphInfos));
}

void SwCanvas::DrawGlyph(FontGlyphs* abstractGlyphs, int glyphIndex, const Graphics::vec2& penPoint)
{
	SwFontGlyphs* glyphs = fast_cast<SwFontGlyphs*>(abstractGlyphs);

	const FontGlyphs::GlyphInfo& glyphInfo = glyphs->GetGlyphInfos()[glyphIndex];

	int destX = (int)penPoint.x + glyphInfo.offsetX;
	int destY = (int)penPoint.y + glyphInfo.offsetY;

	destination->Blit(glyphs->GetImage(), destX, destY, glyphInfo.leftTopX, glyphInfo.leftTopY, glyphInfo.width, glyphInfo.height);
}

END_INANITY_GUI
