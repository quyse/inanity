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
	ptr<VertexLayoutElement> vlePosition;
	ptr<AttributeLayout> al;
	ptr<AttributeLayoutSlot> als;
	ptr<AttributeLayoutElement> alePosition;
	ptr<Instancer> instancer;
	ptr<AttributeBinding> ab;

	Value<vec4> aCorner;

	Interpolant<vec2> iTexcoord;
	Interpolant<vec4> iColor;

	ptr<UniformGroup> ugSymbols;
	UniformArray<vec4> uPositions;
	UniformArray<vec4> uTexcoords;
	UniformArray<vec4> uColors;

	Sampler<float, 2> uFontSampler;
	ptr<SamplerState> ss;

	ptr<VertexBuffer> vb;
	ptr<VertexShader> vs;
	ptr<PixelShader> ps;
	ptr<BlendState> bs;

	Helper(ptr<Device> device, ptr<ShaderCache> shaderCache) :
		vl(NEW(VertexLayout(sizeof(vec4)))),
		vlePosition(vl->AddElement(DataTypes::_vec4, 0)),
		al(NEW(AttributeLayout())),
		als(al->AddSlot()),
		alePosition(al->AddElement(als, vlePosition)),
		instancer(NEW(Instancer(device, maxGlyphsCount, al))),
		ab(device->CreateAttributeBinding(al)),

		aCorner(alePosition),

		iTexcoord(1),
		iColor(2),

		ugSymbols(NEW(UniformGroup(0))),
		uPositions(ugSymbols->AddUniformArray<vec4>(maxGlyphsCount)),
		uTexcoords(ugSymbols->AddUniformArray<vec4>(maxGlyphsCount)),
		uColors(ugSymbols->AddUniformArray<vec4>(maxGlyphsCount)),

		uFontSampler(0)
	{
		BEGIN_TRY();

		ugSymbols->Finalize(device);

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
		Temp<uint> tmpInstance;
		Temp<vec4> tmpPosition, tmpTexcoord, tmpColor;
		vs = shaderCache->GetVertexShader((
			tmpInstance = instancer->GetInstanceID(),

			tmpPosition = uPositions[tmpInstance],
			tmpTexcoord = uTexcoords[tmpInstance],
			tmpColor = uColors[tmpInstance],

			setPosition(newvec4(
				dot(aCorner["xz"], tmpPosition["xz"]),
				dot(aCorner["yw"], tmpPosition["yw"]),
				0, 1)),
			iTexcoord = newvec2(
				dot(aCorner["xz"], tmpTexcoord["xz"]),
				dot(aCorner["yw"], tmpTexcoord["yw"])),
			iColor = tmpColor
		));

		// pixel shader
		ps = shaderCache->GetPixelShader((
			iTexcoord,
			iColor,

			fragment(0, newvec4(iColor["xyz"], iColor["w"] * uFontSampler.Sample(iTexcoord)))
		));

		// blending
		bs = device->CreateBlendState();
		bs->SetColor(BlendState::colorSourceSrcAlpha, BlendState::colorSourceInvSrcAlpha, BlendState::operationAdd);

		// sampling
		{
			SamplerSettings s;
			s.minFilter = s.mipFilter = s.magFilter = SamplerSettings::filterLinear;
			s.wrapU = s.wrapV = s.wrapW = SamplerSettings::wrapClamp;
			ss = device->CreateSamplerState(s);
		}

		END_TRY("Can't create GrCanvas helper");
	}
};

GrCanvas::GrCanvas(ptr<Device> device, ptr<Helper> helper)
: device(device), helper(helper), queuedGlyphsCount(0) {}

void GrCanvas::SetContext(ptr<Context> context)
{
	this->context = context;
}

ptr<GrCanvas> GrCanvas::Create(ptr<Device> device, ptr<ShaderCache> shaderCache)
{
	return NEW(GrCanvas(device, NEW(GrCanvas::Helper(device, shaderCache))));
}

ptr<FontGlyphs> GrCanvas::CreateGlyphs(
	ptr<RawTextureData> image,
	const FontGlyphs::GlyphInfos& glyphInfos
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

	return NEW(GrFontGlyphs(texture, glyphs));

	END_TRY("Can't create graphics font glyphs");
}

void GrCanvas::DrawGlyph(FontGlyphs* abstractGlyphs, int glyphIndex, const vec2& penPoint, const vec4& color)
{
	GrFontGlyphs* glyphs = fast_cast<GrFontGlyphs*>(abstractGlyphs);

	ptr<Texture> texture = glyphs->GetTexture();

	// if queue is full, or current texture is different, flush
	if(queuedGlyphsCount >= Helper::maxGlyphsCount || currentFontTexture != texture)
		Flush();

	currentFontTexture = texture;

	float scaleX = 2.0f / (float)context->GetViewportWidth();
	float scaleY = -2.0f / (float)context->GetViewportHeight();

	const GrFontGlyphs::Glyph& glyph = glyphs->GetGlyphs()[glyphIndex];

	// add glyph to queue
	helper->uPositions.SetValue(queuedGlyphsCount,
		(vec4(penPoint.x, penPoint.y, penPoint.x, penPoint.y) + glyph.offset)
		* vec4(scaleX, scaleY, scaleX, scaleY)
		+ vec4(-1, 1, -1, 1)
	);
	helper->uTexcoords.SetValue(queuedGlyphsCount, glyph.uv);
	helper->uColors.SetValue(queuedGlyphsCount, color);
	++queuedGlyphsCount;
}

void GrCanvas::Flush()
{
	// early exit
	if(!queuedGlyphsCount)
		return;

	Context::LetAttributeBinding lab(context, helper->ab);
	Context::LetVertexBuffer lvb(context, 0, helper->vb);
	Context::LetIndexBuffer lib(context, 0);
	Context::LetVertexShader lvs(context, helper->vs);
	Context::LetPixelShader lps(context, helper->ps);
	Context::LetBlendState lbs(context, helper->bs);
	Context::LetCullMode lcm(context, Context::cullModeNone);
	Context::LetFillMode lfm(context, Context::fillModeSolid);
	Context::LetDepthTestFunc ldtf(context, Context::depthTestFuncAlways);
	Context::LetDepthWrite ldw(context, false);
	Context::LetUniformBuffer lub(context, helper->ugSymbols);
	Context::LetSampler ls(context, helper->uFontSampler, currentFontTexture, helper->ss);

	helper->ugSymbols->Upload(context);

	helper->instancer->Draw(context, queuedGlyphsCount);

	queuedGlyphsCount = 0;
	currentFontTexture = 0;
}

END_INANITY_GUI
