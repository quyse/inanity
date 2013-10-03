#ifndef ___INANITY_GRAPHICS_TEXT_DRAWER_HPP___
#define ___INANITY_GRAPHICS_TEXT_DRAWER_HPP___

#include "graphics.hpp"
#include "FontAlignment.hpp"
#include "../String.hpp"

BEGIN_INANITY_GRAPHICS

struct TextDrawerHelper;
class Font;
class Texture;
class Context;
class Device;
class ShaderCache;

/// Класс рисователя текста.
class TextDrawer : public Object
{
private:
	ptr<TextDrawerHelper> helper;

	/// Текущий графический контекст.
	ptr<Context> context;

	/// Масштабирование.
	/** Вычисляется, исходя из текущего viewport'а в состоянии контекста. */
	float scaleX, scaleY;

	/// Текущее количество символов в очереди.
	int queuedCharsCount;
	/// Текущий шрифт.
	ptr<Font> currentFont;
	/// Последняя установленная в контекст текстура шрифта.
	ptr<Texture> currentFontTexture;

	/// Вывести символ.
	void DrawSymbol(const vec4& position, const vec4& texcoord, const vec4& color);

public:
	TextDrawer(ptr<TextDrawerHelper> helper);

	/// Подготовить графический контекст для рисования текста.
	void Prepare(ptr<Context> context, int viewportWidth, int viewportHeight);

	/// Установить шрифт.
	void SetFont(ptr<Font> font);

	/// Нарисовать строку текста.
	/** Не рисует сразу, а только задаёт текст в очередь на рисование. */
	void DrawTextLine(const String& text, float x, float y, const vec4& color, int alignment);

	/// Очистить очередь на рисование - нарисовать всё, что осталось.
	void Flush();

	/// Создать новый рисователь текста.
	static ptr<TextDrawer> Create(ptr<Device> device, ptr<ShaderCache> shaderCache);
};

END_INANITY_GRAPHICS

#endif
