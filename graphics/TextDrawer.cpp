#include "TextDrawer.hpp"
#include "Font.hpp"
#include "Texture.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "BlendState.hpp"
#include "SamplerState.hpp"
#include "Device.hpp"
#include "Context.hpp"
#include "ShaderCache.hpp"
#include "Layout.hpp"
#include "../MemoryFile.hpp"
#include "../StringTraveler.hpp"
#include "../Exception.hpp"
#include "../inanity-shaders.hpp"

BEGIN_INANITY_GRAPHICS

/// Вспомогательная структура для рисователя.
struct TextDrawerHelper : public Object
{
	/// Максимальное количество символов, рисуемое за раз.
	static const int maxSymbolsCount = 64;

	Attribute<float4> aCorner;

	Interpolant<float4> iPosition;
	Interpolant<float2> iTexcoord;
	Interpolant<float4> iColor;

	Fragment<float4> fTarget;

	ptr<UniformGroup> ugSymbols;
	/// Координаты символов на экране.
	UniformArray<float4> uPositions;
	/// Текстурные координаты символов.
	UniformArray<float4> uTexcoords;
	/// Цвета символов.
	UniformArray<float4> uColors;

	ptr<UniformBuffer> ubSymbols;

	/// Текстура шрифта.
	/** Задаёт альфу для шрифта. */
	Sampler<float, float2> uFontSampler;

	ptr<VertexBuffer> vb;
	ptr<VertexShader> vs;
	ptr<PixelShader> ps;
	ptr<BlendState> bs;

	TextDrawerHelper(ptr<Device> device, ptr<ShaderCache> shaderCache) :
		aCorner(0),

		iPosition(Semantics::VertexPosition),
		iTexcoord(Semantics::Custom(0)),
		iColor(Semantics::Custom(1)),

		fTarget(Semantics::TargetColor0),

		ugSymbols(NEW(UniformGroup(0))),
		uPositions(ugSymbols->AddUniformArray<float4>(maxSymbolsCount)),
		uTexcoords(ugSymbols->AddUniformArray<float4>(maxSymbolsCount)),
		uColors(ugSymbols->AddUniformArray<float4>(maxSymbolsCount)),

		ubSymbols(device->CreateUniformBuffer(ugSymbols->GetSize())),

		uFontSampler(0)
	{
		try
		{
			ugSymbols->Finalize();

			// создать геометрию
			static const float4 vertices[6] =
			{
				float4(0, 1, 1, 0),
				float4(1, 1, 0, 0),
				float4(1, 0, 0, 1),
				float4(0, 1, 1, 0),
				float4(1, 0, 0, 1),
				float4(0, 0, 1, 1)
			};
			ptr<Layout> layout = NEW(Layout(std::vector<Layout::Element>(1, Layout::Element(DataTypes::Float4, 0, 0)), 16));

			vb = device->CreateVertexBuffer(MemoryFile::CreateViaCopy(vertices, sizeof(vertices)), layout);

			// вершинный шейдер
			Temp<uint> tmpInstance;
			Temp<float4> tmpPosition, tmpTexcoord, tmpColor;
			vs = shaderCache->GetVertexShader((
				tmpInstance = Value<uint>(NEW(SpecialNode(DataTypes::UInt, Semantics::Instance))),

				tmpPosition = uPositions[tmpInstance],
				tmpTexcoord = uTexcoords[tmpInstance],
				tmpColor = uColors[tmpInstance],

				iPosition = newfloat4(
					dot(aCorner.Swizzle<float2>("xz"), tmpPosition.Swizzle<float2>("xz")),
					dot(aCorner.Swizzle<float2>("yw"), tmpPosition.Swizzle<float2>("yw")),
					0, 1),
				iTexcoord = newfloat2(
					dot(aCorner.Swizzle<float2>("xz"), tmpTexcoord.Swizzle<float2>("xz")),
					dot(aCorner.Swizzle<float2>("yw"), tmpTexcoord.Swizzle<float2>("yw"))),
				iColor = tmpColor
			));

			// пиксельный шейдер
			ps = shaderCache->GetPixelShader((
				iPosition,
				iTexcoord,
				iColor,

				fTarget = newfloat4(iColor.Swizzle<float3>("xyz"), iColor.Swizzle<float>("w") * uFontSampler.Sample(iTexcoord))
			));

			// настройки смешивания
			bs = device->CreateBlendState();
			bs->SetColor(BlendState::colorSourceSrcAlpha, BlendState::colorSourceInvSrcAlpha, BlendState::operationAdd);

			// настройки семплирования для шрифта
			ptr<SamplerState> ss = device->CreateSamplerState();
			ss->SetFilter(SamplerState::filterLinear, SamplerState::filterLinear, SamplerState::filterLinear);
			ss->SetWrap(SamplerState::wrapBorder, SamplerState::wrapBorder, SamplerState::wrapBorder);
			float color[] = { 0, 0, 0, 0 };
			ss->SetBorderColor(color);
			uFontSampler.SetSamplerState(ss);
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't create text drawer helper", exception);
		}
	}
};

END_INANITY_GRAPHICS

TextDrawer::TextDrawer(ptr<TextDrawerHelper> helper)
: helper(helper), queuedCharsCount(0) {}

void TextDrawer::Prepare(ptr<Context> context)
{
	this->context = context;

	// установить всё, что можно, в состояние контекста
	ContextState& cs = context->GetTargetState();
	cs.vertexBuffer = helper->vb;
	cs.indexBuffer = 0;
	cs.vertexShader = helper->vs;
	cs.pixelShader = helper->ps;
	cs.blendState = helper->bs;
	cs.uniformBuffers[helper->ugSymbols->GetSlot()] = helper->ubSymbols;

	// сбросить текущий шрифт и текстуру
	currentFont = 0;
	currentFontTexture = 0;

	// вычислить масштабирование
	scaleX = 2.0f / cs.viewportWidth;
	scaleY = 2.0f / cs.viewportHeight;
}

void TextDrawer::SetFont(ptr<Font> font)
{
	// запомнить шрифт
	currentFont = font;
	// если текстура поменялась
	ptr<Texture> texture = font->GetTexture();
	if(texture != currentFontTexture)
	{
		// сбросить очередь
		Flush();
		// запомнить новую текстуру
		currentFontTexture = texture;
	}
}

void TextDrawer::DrawSymbol(const float4& position, const float4& texcoord, const float4& color)
{
	// если очередь переполнена
	if(queuedCharsCount >= TextDrawerHelper::maxSymbolsCount)
		// сбросить очередь
		Flush();

	// добавить символ в очередь
	helper->uPositions.SetValue(queuedCharsCount, position);
	helper->uTexcoords.SetValue(queuedCharsCount, texcoord);
	helper->uColors.SetValue(queuedCharsCount, color);
	++queuedCharsCount;
}

void TextDrawer::DrawTextLine(const String& text, float x, float y, const float4& color, int alignment)
{
	Font* font = currentFont;

	// получить данные шрифта
	const Font::Charset& charset = font->GetCharset();
	const Font::KerningPairs& kerningPairs = font->GetKerningPairs();

	//получить высоту символов
	float charHeight = font->GetCharHeight() * scaleY;

	//определить ширину строки
	float stringWidth = 0;
	{
		StringTraveler traveler(text);
		wchar_t symbol, lastSymbol = (wchar_t)-1;
		while(traveler.Next(symbol))
		{
			// если это не первый символ в строке
			if(lastSymbol != (wchar_t)-1)
				// добавить кернинг
				stringWidth += font->GetKerning(lastSymbol, symbol);
			// добавить ширину символа
			stringWidth += font->GetChar(symbol).width;
			// запомнить символ для следующей кернинг-пары
			lastSymbol = symbol;
		}
		// масштабирование
		stringWidth *= scaleX;
	}

	//определить высоту строки (пока просто - многострочный текст не поддерживается)
	float stringHeight = charHeight;

	//установить реальные x и y в соответствии с выравниванием
	//для FontAlignment::Left ничего не надо
	switch(alignment & FontAlignments::HorizontalMask)
	{
	case FontAlignments::Center:
		x -= stringWidth * 0.5f;
		break;
	case FontAlignments::Right:
		x -= stringWidth;
		break;
	}
	//для FontAlignment::Bottom ничего не надо
	switch(alignment & FontAlignments::VerticalMask)
	{
	case FontAlignments::Top:
		y -= stringHeight;
		break;
	case FontAlignments::Middle:
		y -= stringHeight * 0.5f;
		break;
	}

	//левая граница
	float left = x;

	//цикл обработки символов
	StringTraveler traveler(text);
	wchar_t symbol, lastSymbol = (wchar_t)-1;
	while(traveler.Next(symbol))
	{
		// если это не первый символ
		if(lastSymbol != (wchar_t)-1)
			// сдвинуть левую границу на кернинг
			left += font->GetKerning(lastSymbol, symbol) * scaleX;
		//получить символ
		const FontChar& fontChar = font->GetChar(symbol);
		//получить ширину символа
		float charWidth = fontChar.width * scaleX;

		// вывести символ
		DrawSymbol(
			float4(
				left + fontChar.screenFirstUV.x * scaleX, y + fontChar.screenFirstUV.y * scaleY,
				left + fontChar.screenSecondUV.x * scaleX, y + fontChar.screenSecondUV.y * scaleY
			),
			float4(fontChar.firstUV.x, fontChar.firstUV.y, fontChar.secondUV.x, fontChar.secondUV.y),
			color
		);

		//сдвинуть левую границу
		left += charWidth;

		// запомнить символ
		lastSymbol = symbol;
	}
}

void TextDrawer::Flush()
{
	// если очередь пуста, делать нечего
	if(!queuedCharsCount)
		return;

	// установить текстуру в контекст
	helper->uFontSampler.SetTexture(currentFontTexture);
	helper->uFontSampler.Apply(context->GetTargetState());

	// загрузить символы
	context->SetUniformBufferData(helper->ubSymbols, helper->ugSymbols->GetData(), helper->ugSymbols->GetSize());

	// нарисовать
	context->DrawInstanced(queuedCharsCount);

	// сбросить количество символов
	queuedCharsCount = 0;
}

ptr<TextDrawer> TextDrawer::Create(ptr<Device> device, ptr<ShaderCache> shaderCache)
{
	return NEW(TextDrawer(NEW(TextDrawerHelper(device, shaderCache))));
}
