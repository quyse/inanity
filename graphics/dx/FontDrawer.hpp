#ifndef ___INANITY_DX_FONT_DRAWER_HPP___
#define ___INANITY_DX_FONT_DRAWER_HPP___

#include "dx.hpp"
#include "../FontAlignment.hpp"
#include "../../String.hpp"

BEGIN_INANITY

class ResourceLoader;

END_INANITY

BEGIN_INANITY_GRAPHICS

class Font;

END_INANITY_GRAPHICS

BEGIN_INANITY_DX

/// Структура данных для шейдера.
struct FontDrawerShaderData
{
	/// Максимальное количество символов, рисуемое в один проход.
	static const size_t maximumSymbols = 64;

	/// Структура символа.
	struct Symbol
	{
		float4 texcoord;
		float4 position;
		float4 color;
	};
	/// Символы.
	Symbol symbols[maximumSymbols];
};

class Geometry;
class ShaderLayout;
class VertexShader;
class PixelShader;
class SamplerState;
class ConstantBuffer;
class Context;

/// Класс рисователя шрифта.
/** Шрифт рисуется конкретным шейдером. Шейдеру передаётся одна текстура,
а также по 3 вектора на символ.
FontDrawer является статическим ресурсом. */
class FontDrawer : public Object
{
public:
	static String staticResourceName;

private:
	ptr<Geometry> geometry;
	ptr<ShaderLayout> shaderLayout;
	ptr<VertexShader> vertexShader;
	ptr<PixelShader> pixelShader;
	ptr<SamplerState> samplerState;
	/// Графический буфер для параметров шейдера.
	ptr<ConstantBuffer> constantBuffer;
	/// Текущий шрифт.
	Font* font;
	/// Поправки на экран.
	float scaleX, scaleY;

public:
	FontDrawer(ptr<Geometry> geometry, ptr<ShaderLayout> shaderLayout, ptr<VertexShader> vertexShader,
		ptr<PixelShader> pixelShader, ptr<SamplerState> samplerState, ptr<ConstantBuffer> constantBuffer);

	/// Подготовиться к рисованию.
	void Prepare(Context* context, float scaleX, float scaleY);
	/// Установить шрифт.
	void PrepareFont(Context* context, Font* font);
	/// Напечатать однострочный текст.
	void DrawTextLine(Context* context, const String& text, float x, float y, const float4& color, int alignment = FontAlignment::Left | FontAlignment::Bottom);

	static ptr<FontDrawer> CreateStaticResource(ResourceLoader* resourceLoader);
};

END_INANITY_DX

#endif
