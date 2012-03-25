#ifndef ___INANITY_EDITABLE_FONT_HPP___
#define ___INANITY_EDITABLE_FONT_HPP___

#include "graphics.hpp"
#include "FontChar.hpp"
#include "../String.hpp"
#include <unordered_map>

BEGIN_INANITY

class OutputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

class EditableFont : public Object
{
private:
	//текстура
	String textureName;
	//набор символов
	std::unordered_map<wchar_t, FontChar> charset;
	//высота символов
	float charHeight;

public:
	EditableFont(const String& textureName, const std::unordered_map<wchar_t, FontChar>& charset, float charHeight);

	String GetTextureName() const;
	void SetTextureName(const String& textureName);

	std::unordered_map<wchar_t, FontChar>& GetCharset();

	float GetCharHeight() const;
	void SetCharHeight(float charHeight);

	void Serialize(ptr<OutputStream> outputStream);
};

END_INANITY_GRAPHICS

#endif
