#ifndef ___INANITY_EDITABLE_FONT_HPP___
#define ___INANITY_EDITABLE_FONT_HPP___

#include "graphics.hpp"
#include "FontChar.hpp"
#include "../String.hpp"
#include <unordered_map>
#include <map>

BEGIN_INANITY

class OutputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Редактируемый шрифт.
class EditableFont : public Object
{
public:
	typedef std::unordered_map<wchar_t, FontChar> Charset;
	typedef std::map<std::pair<wchar_t, wchar_t>, float> KerningPairs;

private:
	/// Имя ресурса текстуры.
	String textureName;
	/// Набор символов.
	Charset charset;
	/// Набор кернинг-пар для лучшего выравнивания текста.
	KerningPairs kerningPairs;
	/// высота символов
	float charHeight;

public:
	EditableFont(const String& textureName, const Charset& charset, const KerningPairs& kerningPairs, float charHeight);

	String GetTextureName() const;
	void SetTextureName(const String& textureName);

	Charset& GetCharset();
	KerningPairs& GetKerningPairs();

	float GetCharHeight() const;
	void SetCharHeight(float charHeight);

	void Serialize(ptr<OutputStream> outputStream);
};

END_INANITY_GRAPHICS

#endif
