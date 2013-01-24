#include "Font.hpp"
#include "Texture.hpp"
#include "TextureManager.hpp"
#include "../InputStream.hpp"
#include "../StreamReader.hpp"
#include "../ResourceManager.ipp"
#include "../Exception.hpp"

FontChar Font::defaultFontChar;

Font::Font(ptr<Texture> texture, const Charset& charset, const KerningPairs& kerningPairs, float charHeight)
: texture(texture), charset(charset), kerningPairs(kerningPairs), charHeight(charHeight)
{
}

ptr<Texture> Font::GetTexture() const
{
	return texture;
}

Font::Charset& Font::GetCharset()
{
	return charset;
}

Font::KerningPairs& Font::GetKerningPairs()
{
	return kerningPairs;
}

float Font::GetCharHeight() const
{
	return charHeight;
}

const FontChar& Font::GetChar(wchar_t symbol)
{
	Charset::const_iterator i = charset.find(symbol);
	if(i == charset.end())
		i = charset.find(0);
	return (i == charset.end()) ? defaultFontChar : i->second;
}

float Font::GetKerning(wchar_t first, wchar_t second)
{
	KerningPairs::const_iterator i = kerningPairs.find(first | (second << 16));
	return (i == kerningPairs.end()) ? 0 : i->second;
}

ptr<Font> Font::Deserialize(ptr<InputStream> inputStream, ptr<TextureManager> textureManager)
{
	try
	{
		ptr<StreamReader> reader = NEW(StreamReader(inputStream));

		// считать заголовок файла
		String textureName = reader->ReadString();
		float charHeight = reader->Read<float>();

		// получить текстуру
		ptr<Texture> texture = textureManager->Get(textureName);

		// считать символы
		size_t charsetSize = reader->ReadShortly();
		Charset charset;
		for(size_t i = 0; i < charsetSize; ++i)
		{
			std::pair<wchar_t, FontChar> symbol;
			symbol.first = reader->Read<wchar_t>();
			symbol.second = reader->Read<FontChar>();
			charset.insert(symbol);
		}

		// считать кернинг-пары
		size_t kerningPairsCount = reader->ReadShortly();
		KerningPairs kerningPairs;
		for(size_t i = 0; i < kerningPairsCount; ++i)
		{
			std::pair<unsigned, float> kerningPair;
			kerningPair.first = reader->Read<wchar_t>();
			kerningPair.first |= reader->Read<wchar_t>() << 16;
			kerningPair.second = reader->Read<float>();
			kerningPairs.insert(kerningPair);
		}

		// вернуть шрифт
		return NEW(Font(texture, charset, kerningPairs, charHeight));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't deserialize font", exception);
	}
}
