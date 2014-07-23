#include "SwCanvas.hpp"
#include "SwFontGlyphs.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GUI

using namespace Graphics;

void SwCanvas::SetDestination(ptr<RawTextureData> destination)
{
	if(destination->GetPixelSize() != 3)
		THROW("SwCanvas supports only RGB-byte destination images");
	this->destination = destination;
}

ptr<FontGlyphs> SwCanvas::CreateGlyphs(
	const FontGlyphs::GlyphInfos& glyphInfos,
	float originalFontSize,
	FontImageType fontImageType,
	float smoothCoef,
	ptr<Graphics::RawTextureData> image
)
{
	return NEW(SwFontGlyphs(glyphInfos, originalFontSize, fontImageType, smoothCoef, image));
}

void SwCanvas::DrawGlyph(
	FontGlyphs* abstractGlyphs,
	int glyphIndex,
	const vec2& penPoint,
	float scale,
	const vec4& color
)
{
	SwFontGlyphs* glyphs = fast_cast<SwFontGlyphs*>(abstractGlyphs);

	const FontGlyphs::GlyphInfo& glyphInfo = glyphs->GetGlyphInfos()[glyphIndex];

	if(glyphInfo.width == 0 || glyphInfo.height == 0)
		return;

	float x1 = penPoint.x + (float)glyphInfo.offsetX * scale;
	float y1 = penPoint.y + (float)glyphInfo.offsetY * scale;
	float x2 = x1 + (float)glyphInfo.width * scale;
	float y2 = y1 + (float)glyphInfo.height * scale;

	// calculate UV transform
	float uX = (float)glyphInfo.width / (x2 - x1);
	float uC = -uX * x1 + (float)glyphInfo.leftTopX;
	float vY = (float)glyphInfo.height / (y2 - y1);
	float vC = -vY * y1 + (float)glyphInfo.leftTopY;

	// clip drawing by destination
	int left = std::max((int)floor(x1), 0);
	int top = std::max((int)floor(y1), 0);
	int right = std::min((int)ceil(x2), destination->GetImageWidth());
	int bottom = std::min((int)ceil(y2), destination->GetImageHeight());

	RawTextureData* glyphImage = glyphs->GetImage();
	int glyphPitch = glyphImage->GetMipLinePitch();
	unsigned char* glyphData = (unsigned char*)glyphImage->GetMipData();

	auto getGlyphData = [glyphData, glyphPitch, &glyphInfo](int i, int j) -> float
	{
		if(i < glyphInfo.leftTopY || i >= glyphInfo.leftTopY + glyphInfo.height || j < glyphInfo.leftTopX || j >= glyphInfo.leftTopX + glyphInfo.width)
			return 0.0f;
		return (float)glyphData[glyphPitch * i + j];
	};

	bool distanceField = glyphs->GetFontImageType() == FontImageTypes::distanceField;
	float smooth = glyphs->GetSmoothCoef() * scale;

	int destPitch = destination->GetMipLinePitch();
	unsigned char* destData = (unsigned char*)destination->GetMipData();

	for(int y = top; y < bottom; ++y)
	{
		float v = vY * (float)y + vC;
		int vi = (int)v;
		float vv = v - (float)vi;

		for(int x = left; x < right; ++x)
		{
			float u = uX * (float)x + uC;
			int ui = (int)u;
			float uu = u - (float)ui;

			// bilinear interpolation
			float alpha = (
				getGlyphData(vi, ui) * (1.0f - uu) +
				getGlyphData(vi, ui + 1) * uu
				//(float)glyphData[glyphPitch * vi + ui] * (1.0f - uu) +
				//(float)glyphData[glyphPitch * vi + ui + 1] * uu
			) * (1.0f - vv) +
			(
				getGlyphData(vi + 1, ui) * (1.0f - uu) +
				getGlyphData(vi + 1, ui + 1) * uu
				//(float)glyphData[glyphPitch * (vi + 1) + ui] * (1.0f - uu) +
				//(float)glyphData[glyphPitch * (vi + 1) + ui + 1] * uu
			) * vv;

			alpha /= 255;

			// apply distance field transform
			if(distanceField)
				alpha = std::min(std::max((alpha - 0.5f) * smooth + 0.5f, 0.0f), 1.0f);

			alpha *= color.w;

			// alpha blend
			for(int z = 0; z < 3; ++z)
			{
				unsigned char& component = destData[y * destPitch + x * 3 + z];
				component = (unsigned char)((float)component * (1.0f - alpha) + color(z) * 255.0f * alpha);
			}
		}
	}
}

END_INANITY_GUI
