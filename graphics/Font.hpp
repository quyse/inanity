#ifndef ___INANITY_FONT_HPP___
#define ___INANITY_FONT_HPP___

#include "graphics.hpp"
#include "FontChar.hpp"
#include "../String.hpp"
#include <unordered_map>
#include <map>

BEGIN_INANITY

class InputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

class Texture;
class TextureManager;

/// Raster font.
class Font : public Object
{
public:
	typedef std::unordered_map<wchar_t, FontChar> Charset;
	/// Kerning pairs.
	typedef std::map<std::pair<wchar_t, wchar_t>, float> KerningPairs;

private:
	/// Текстура.
	ptr<Texture> texture;
	/// Набор символов.
	Charset charset;
	/// Набор кернинг-пар для лучшего выравнивания текста.
	KerningPairs kerningPairs;
	/// высота символов
	float charHeight;

	/// Символ по умолчанию.
	/** Применяется, только если шрифт не имеет нулевого символа. */
	static FontChar defaultFontChar;

public:
	Font(ptr<Texture> texture, const Charset& charset, const KerningPairs& kerningPairs, float charHeight);

	ptr<Texture> GetTexture() const;

	Charset& GetCharset();
	KerningPairs& GetKerningPairs();

	float GetCharHeight() const;

	/// Получить символ.
	/** Если символа нет в наборе, возвращается символ по умолчанию. */
	const FontChar& GetChar(wchar_t symbol);
	/// Получить кернинг (расстояние между двумя символами).
	/** Если кернинг-пары нет, то возвращается 0. */
	float GetKerning(wchar_t first, wchar_t second);

	/// Загрузить шрифт.
	static ptr<Font> Deserialize(ptr<InputStream> inputStream, ptr<TextureManager> textureManager);
};

END_INANITY_GRAPHICS

#endif
