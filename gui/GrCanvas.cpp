#include "GrCanvas.hpp"
#include "GrFontGlyphs.hpp"
#include "../graphics/Device.hpp"
#include "../graphics/Context.hpp"
#include "../graphics/Texture.hpp"
#include "../graphics/SamplerSettings.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../graphics/UniformBuffer.hpp"
#include "../graphics/VertexBuffer.hpp"
#include "../graphics/IndexBuffer.hpp"
#include "../graphics/VertexShader.hpp"
#include "../graphics/PixelShader.hpp"
#include "../graphics/BlendState.hpp"
#include "../graphics/SamplerState.hpp"
#include "../graphics/ShaderCache.hpp"
#include "../graphics/VertexLayout.hpp"
#include "../graphics/VertexLayoutElement.hpp"
#include "../graphics/AttributeLayout.hpp"
#include "../graphics/AttributeLayoutSlot.hpp"
#include "../graphics/AttributeLayoutElement.hpp"
#include "../graphics/AttributeBinding.hpp"
#include "../graphics/RenderBuffer.hpp"
#include "../graphics/DepthStencilBuffer.hpp"
#include "../inanity-shaders.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GUI

using namespace Graphics;
using namespace Graphics::Shaders;

struct GrCanvas::Helper : public Object
{
	/// Max number of glyphs which could be drawn at once.
	static const int maxGlyphsCount = 64;

	ptr<VertexLayout> vl;
	ptr<VertexLayoutElement> vleCorner;
	ptr<AttributeLayout> al;
	ptr<AttributeLayoutSlot> als;
	ptr<AttributeLayoutElement> aleCorner;
	ptr<Instancer> instancer;
	ptr<AttributeBinding> ab;

	Value<vec4> aCorner;

	Interpolant<vec2> iTexcoord;
	Interpolant<vec4> iColor;

	ptr<UniformGroup> ug;
	UniformArray<vec4> uPositions;
	UniformArray<vec4> uTexcoords;
	UniformArray<vec4> uColors;
	Uniform<float> uSmoothCoef;

	Sampler<float, 2> uFontSampler;

	ptr<VertexBuffer> vb;
	ptr<VertexShader> vs;
	ptr<PixelShader> psGrayscale;
	ptr<PixelShader> psDistanceField;
	ptr<BlendState> bs;

	Helper(ptr<Device> device, ptr<ShaderCache> shaderCache) :
		vl(NEW(VertexLayout(sizeof(vec4)))),
		vleCorner(vl->AddElement(DataTypes::_vec4, 0)),
		al(NEW(AttributeLayout())),
		als(al->AddSlot()),
		aleCorner(al->AddElement(als, vleCorner)),
		instancer(NEW(Instancer(device, maxGlyphsCount, al))),
		ab(device->CreateAttributeBinding(al)),

		aCorner(aleCorner),

		iTexcoord(1),
		iColor(2),

		ug(NEW(UniformGroup(0))),
		uPositions(ug->AddUniformArray<vec4>(maxGlyphsCount)),
		uTexcoords(ug->AddUniformArray<vec4>(maxGlyphsCount)),
		uColors(ug->AddUniformArray<vec4>(maxGlyphsCount)),
		uSmoothCoef(ug->AddUniform<float>()),

		uFontSampler(0)
	{
		BEGIN_TRY();

		ug->Finalize(device);

		// geometry
		static const vec4 vertices[6] =
		{
			vec4(0, 1, 1, 0),
			vec4(1, 1, 0, 0),
			vec4(1, 0, 0, 1),
			vec4(0, 1, 1, 0),
			vec4(1, 0, 0, 1),
			vec4(0, 0, 1, 1)
		};

		vb = device->CreateStaticVertexBuffer(MemoryFile::CreateViaCopy(vertices, sizeof(vertices)), vl);

		// vertex shader
		Value<uint> tmpInstance = instancer->GetInstanceID();
		Value<vec4> tmpPosition = uPositions[tmpInstance];
		Value<vec4> tmpTexcoord = uTexcoords[tmpInstance];
		Value<vec4> tmpColor = uColors[tmpInstance];
		vs = shaderCache->GetVertexShader((
			setPosition(newvec4(
				dot(aCorner["xz"], tmpPosition["xz"]),
				dot(aCorner["yw"], tmpPosition["yw"]),
				0, 1)),
			iTexcoord.Set(newvec2(
				dot(aCorner["xz"], tmpTexcoord["xz"]),
				dot(aCorner["yw"], tmpTexcoord["yw"]))),
			iColor.Set(tmpColor)
		));

		// pixel shaders
		psGrayscale = shaderCache->GetPixelShader(
			fragment(0, newvec4(iColor["xyz"], iColor["w"] * uFontSampler.Sample(iTexcoord)))
		);
		psDistanceField = shaderCache->GetPixelShader(
			fragment(0, newvec4(iColor["xyz"], iColor["w"] * saturate((uFontSampler.Sample(iTexcoord) - val(0.5f)) * uSmoothCoef + val(0.5f))))
		);

		// blending
		bs = device->CreateBlendState();
		bs->SetColor(BlendState::colorSourceSrcAlpha, BlendState::colorSourceInvSrcAlpha, BlendState::operationAdd);

		END_TRY("Can't create GrCanvas helper");
	}
};

GrCanvas::GrCanvas(ptr<Device> device, ptr<Helper> helper)
: device(device), helper(helper), queuedGlyphsCount(0), currentSmoothCoef(-1), currentDistanceFieldMode(false) {}

void GrCanvas::SetContext(ptr<Context> context)
{
	this->context = context;
}

ptr<GrCanvas> GrCanvas::Create(ptr<Device> device, ptr<ShaderCache> shaderCache)
{
	return NEW(GrCanvas(device, NEW(GrCanvas::Helper(device, shaderCache))));
}

ptr<FontGlyphs> GrCanvas::CreateGlyphs(
	const FontGlyphs::GlyphInfos& glyphInfos,
	float originalFontSize,
	FontImageType fontImageType,
	float smoothCoef,
	ptr<Graphics::RawTextureData> image
)
{
	BEGIN_TRY();

	SamplerSettings samplerSettings;
	samplerSettings.SetFilter(SamplerSettings::filterLinear);
	samplerSettings.SetWrap(SamplerSettings::wrapClamp);
	ptr<Texture> texture = device->CreateStaticTexture(image, samplerSettings);

	vec2 invSize(1.0f / float(image->GetImageWidth()), 1.0f / float(image->GetImageHeight()));

	GrFontGlyphs::Glyphs glyphs(glyphInfos.size());
	for(size_t i = 0; i < glyphs.size(); ++i)
	{
		GrFontGlyphs::Glyph& glyph = glyphs[i];
		const FontGlyphs::GlyphInfo& glyphInfo = glyphInfos[i];

		glyph.uv = vec4(
			float(glyphInfo.leftTopX),
			float(glyphInfo.leftTopY + glyphInfo.height),
			float(glyphInfo.leftTopX + glyphInfo.width),
			float(glyphInfo.leftTopY)
			) * vec4(invSize.x, invSize.y, invSize.x, invSize.y);
		glyph.offset = vec4(
			float(glyphInfo.offsetX),
			float(glyphInfo.offsetY + glyphInfo.height),
			float(glyphInfo.offsetX + glyphInfo.width),
			float(glyphInfo.offsetY)
			);
	}

	return NEW(GrFontGlyphs(glyphInfos, originalFontSize, fontImageType, smoothCoef, texture, glyphs));

	END_TRY("Can't create graphics font glyphs");
}

void GrCanvas::DrawGlyph(
	FontGlyphs* abstractGlyphs,
	int glyphIndex,
	const vec2& penPoint,
	float scale,
	const vec4& color
)
{
	GrFontGlyphs* glyphs = fast_cast<GrFontGlyphs*>(abstractGlyphs);

	ptr<Texture> texture = glyphs->GetTexture();
	float smoothCoef = glyphs->GetSmoothCoef() * scale * 2;
	bool distanceFieldMode = glyphs->GetFontImageType() == FontImageTypes::distanceField;

	// if queue is full, or some parameter is different, flush
	if(queuedGlyphsCount >= Helper::maxGlyphsCount
		|| currentFontTexture != texture
		|| currentSmoothCoef != smoothCoef
		|| currentDistanceFieldMode != distanceFieldMode
		)
		Flush();

	currentFontTexture = texture;
	currentSmoothCoef = smoothCoef;
	currentDistanceFieldMode = distanceFieldMode;

	float scaleX = 2.0f / (float)context->GetViewportWidth();
	float scaleY = -2.0f / (float)context->GetViewportHeight();

	const GrFontGlyphs::Glyph& glyph = glyphs->GetGlyphs()[glyphIndex];

	// add glyph to queue
	helper->uPositions.Set(queuedGlyphsCount,
		(vec4(penPoint.x, penPoint.y, penPoint.x, penPoint.y) + glyph.offset * scale)
		* vec4(scaleX, scaleY, scaleX, scaleY)
		+ vec4(-1, 1, -1, 1)
	);
	helper->uTexcoords.Set(queuedGlyphsCount, glyph.uv);
	helper->uColors.Set(queuedGlyphsCount, color);
	++queuedGlyphsCount;
}

void GrCanvas::Flush()
{
	// early exit
	if(!queuedGlyphsCount)
		return;

	helper->uSmoothCoef.Set(currentSmoothCoef);

	Context::LetAttributeBinding lab(context, helper->ab);
	Context::LetVertexBuffer lvb(context, 0, helper->vb);
	Context::LetIndexBuffer lib(context, 0);
	Context::LetVertexShader lvs(context, helper->vs);
	Context::LetPixelShader lps(context, currentDistanceFieldMode ? helper->psDistanceField : helper->psGrayscale);
	Context::LetBlendState lbs(context, helper->bs);
	Context::LetCullMode lcm(context, Context::cullModeNone);
	Context::LetFillMode lfm(context, Context::fillModeSolid);
	Context::LetDepthTestFunc ldtf(context, Context::depthTestFuncAlways);
	Context::LetDepthWrite ldw(context, false);
	Context::LetUniformBuffer lub(context, helper->ug);
	Context::LetSampler ls(context, helper->uFontSampler, currentFontTexture);

	helper->ug->Upload(context);

	helper->instancer->Draw(context, queuedGlyphsCount);

	queuedGlyphsCount = 0;
	currentFontTexture = 0;
}

END_INANITY_GUI
