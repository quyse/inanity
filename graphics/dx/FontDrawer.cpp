#include "FontDrawer.hpp"
#include "../Font.hpp"
#include "../../ResourceLoader.hpp"
#include "../../ResourceManager.hpp"
#include "../Geometry.hpp"
#include "ShaderLayout.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "SamplerState.hpp"
#include "ConstantBuffer.hpp"
#include "Context.hpp"
#include "../EditableGeometry.hpp"
#include "Texture.hpp"
#include "ShaderCompiler.hpp"
#include "../ShaderSource.hpp"
#include "ShaderReflection.hpp"
#include "../../Strings.hpp"
#include "../../StringTraveler.hpp"
#include "../../MemoryStream.hpp"
#include "../../StreamWriter.hpp"
#include "../../TempFileSystem.hpp"
#include "../../PartFile.hpp"
#include "../../Exception.hpp"

String FontDrawer::staticResourceName = "/inanity/dx/FontDrawer";

FontDrawer::FontDrawer(ptr<Geometry> geometry, ptr<ShaderLayout> shaderLayout, ptr<VertexShader> vertexShader,
	ptr<PixelShader> pixelShader, ptr<SamplerState> samplerState, ptr<ConstantBuffer> constantBuffer)
	: geometry(geometry), shaderLayout(shaderLayout), vertexShader(vertexShader), pixelShader(pixelShader),
	samplerState(samplerState), constantBuffer(constantBuffer), scaleX(1), scaleY(1)
{
}

void FontDrawer::Prepare(Context* context, float scaleX, float scaleY)
{
	context->ApplyGeometry(geometry);
	context->ApplyShaderLayout(shaderLayout);
	context->ApplyShaders(vertexShader, pixelShader);
	context->SetConstantBuffer(0, Context::stageVertex, constantBuffer);
	context->SetSamplerState(0, Context::stagePixel, samplerState);

	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

void FontDrawer::PrepareFont(Context* context, Font* font)
{
	context->SetTexture(0, Context::stagePixel, font->GetTexture());
	this->font = font;
}

void FontDrawer::DrawTextLine(Context* context, const String& text, float x, float y, const float4& color, int alignment)
{
	// получить данные шрифта
	const Font::Charset& charset = font->GetCharset();
	const Font::KerningPairs& kerningPairs = font->GetKerningPairs();

	//сформировать данные для шейдера
	FontDrawerShaderData shaderData;

	//получить высоту символов
	float charHeight = font->GetCharHeight() * scaleY;

	//определить ширину строки
	//float stringWidth = font->GetStringWidth(text) * scaleX;
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
	switch(alignment & FontAlignment::HorizontalMask)
	{
	case FontAlignment::CenterX:
		x -= stringWidth * 0.5f;
		break;
	case FontAlignment::Right:
		x -= stringWidth;
		break;
	}
	//для FontAlignment::Bottom ничего не надо
	switch(alignment & FontAlignment::VerticalMask)
	{
	case FontAlignment::Top:
		y -= stringHeight;
		break;
	case FontAlignment::CenterY:
		y -= stringHeight * 0.5f;
		break;
	}

	//левая граница
	float left = x;
	//счетчик символов в буфере
	unsigned charsCount = 0;

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

		//добавить символ к массиву
		shaderData.symbols[charsCount].texcoord = float4(fontChar.firstUV.x, fontChar.firstUV.y, fontChar.secondUV.x, fontChar.secondUV.y);
		shaderData.symbols[charsCount].position = float4(
			left + fontChar.screenFirstUV.x * scaleX, y + fontChar.screenFirstUV.y * scaleY,
			left + fontChar.screenSecondUV.x * scaleX, y + fontChar.screenSecondUV.y * scaleY);
		shaderData.symbols[charsCount].color = color;

		//сдвинуть левую границу
		left += charWidth;

		// запомнить символ
		lastSymbol = symbol;

		//если символов слишком много
		if(++charsCount >= FontDrawerShaderData::maximumSymbols)
		{
			//нарисовать буфер и идти дальше
			context->SetBufferData(constantBuffer, &shaderData, sizeof(shaderData));
			context->DrawGeometry(6, false, charsCount);

			//очистить буфер
			charsCount = 0;
		}
	}

	//если в буфере остались символы
	if(charsCount)
	{
		//нарисовать буфер
		context->SetBufferData(constantBuffer, &shaderData, sizeof(shaderData));
		context->DrawGeometry(6, false, charsCount);
	}
}

// ******* Далее дурацкий код, загружающий ресурсы.

static String codeGeneral = "\
struct Vertex2Pixel\
{\
	float4 position : SV_Position;\
	float2 textureUV : TEXCOORD;\
	float4 color : COLOR;\
};";

static String codeVS = codeGeneral + "\
cbuffer Parameters : register(b0) { float4 symbols[64 * 3]; }; \n\
struct VertexInput { float4 corner : CORNER; }; \n\
\
Vertex2Pixel VS(VertexInput input, uint instanceID : SV_InstanceID) \n\
{\n\
	Vertex2Pixel output;\n\
\n\
	float4 t = symbols[instanceID * 3];\n\
	float4 p = symbols[instanceID * 3 + 1];\n\
	float4 c = symbols[instanceID * 3 + 2];\n\
	output.position = float4(dot(input.corner.xz, p.xz), dot(input.corner.yw, p.yw), 0, 1);\n\
	output.textureUV = float4(dot(input.corner.xz, t.xz), dot(input.corner.yw, t.yw), 0, 1);\n\
	output.color = c;\n\
\n\
	return output;\n\
}";

static String codePS = codeGeneral + "\
Texture2D fontTexture : register(t0);\n\
SamplerState fontSampler : register(s0);\n\
\n\
struct PixelOutput { float4 target : SV_Target; };\n\
\n\
PixelOutput PS(Vertex2Pixel input)\n\
{\n\
	PixelOutput output;\n\
	output.target = float4(input.color.xyz, fontTexture.Sample(fontSampler, input.textureUV).x * input.color.w);\n\
	return output;\n\
}";

//инициализация для вершин геометрии символа
static float4 verticesInitialization[6] =
{
	float4(0, 1, 1, 0),
	float4(1, 1, 0, 0),
	float4(1, 0, 0, 1),
	float4(0, 1, 1, 0),
	float4(1, 0, 0, 1),
	float4(0, 0, 1, 1)
};

ptr<FontDrawer> FontDrawer::CreateStaticResource(ResourceLoader* resourceLoader)
{
	try
	{
		// добавить специальные ресурсы

		ptr<ResourceManager> resourceManager = resourceLoader->GetResourceManager();

		ptr<TempFileSystem> tempFileSystem = resourceLoader->GetStaticResource<TempFileSystem>();

		// формат геометрии
		{
			std::vector<std::pair<String, GeometrySemantic> > semantics(1);
			semantics[0] = std::pair<String, GeometrySemantic>("CORNER0", GeometrySemantic(GeometrySemantic::typeFloat, 4, 0));
			ptr<GeometryFormat> geometryFormat = NEW(GeometryFormat(semantics));
			ptr<MemoryStream> stream = NEW(MemoryStream());
			geometryFormat->Serialize(stream);
			tempFileSystem->SaveFile(stream->ToFile(), "/dx/FontDrawerGeometryFormat");
		}
		// геометрия
		{
			ptr<EditableGeometry<float4> > geometry = NEW(EditableGeometry<float4>(std::vector<float4>(verticesInitialization, verticesInitialization + 6)));

			ptr<MemoryStream> stream = NEW(MemoryStream());
			geometry->Serialize(stream, "FontDrawerGeometryFormat");
			tempFileSystem->SaveFile(stream->ToFile(), "/dx/FontDrawerGeometry");
		}

		// наконец-то загрузка
		ptr<Geometry> geometry = resourceLoader->LoadResource<Geometry>("/inanity/dx/FontDrawerGeometry");

		ptr<ShaderCompiler> shaderCompiler = NEW(ShaderCompiler());

		ptr<System> system = resourceLoader->GetStaticResource<System>();

		ptr<File> vsFile = shaderCompiler->Compile(NEW(ShaderSource(Strings::String2File(codeVS), nullptr, "VS", "vs_4_0")));
		ptr<VertexShader> vertexShader = system->CreateVertexShader(vsFile);
		ptr<ShaderLayout> shaderLayout = MakePointer(NEW(ShaderReflection(vsFile)))->CreateShaderLayout(system, geometry->GetFormat());
		ptr<PixelShader> pixelShader = system->CreatePixelShader(shaderCompiler->Compile(NEW(ShaderSource(Strings::String2File(codePS), nullptr, "PS", "ps_4_0"))));

		ptr<ConstantBuffer> constantBuffer = system->CreateConstantBuffer(sizeof(FontDrawerShaderData));

		ptr<SamplerState> samplerState = NEW(SamplerState(resourceLoader->LoadStaticResource<SamplerStateCache>()));
		samplerState->SetFilter(D3D11_FILTER_MIN_MAG_MIP_POINT);

		return NEW(FontDrawer(geometry, shaderLayout, vertexShader, pixelShader, samplerState, constantBuffer));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create static font drawer", exception);
	}
}
