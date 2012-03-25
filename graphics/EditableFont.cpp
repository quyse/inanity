#include "EditableFont.hpp"
#include "../OutputStream.hpp"
#include "../StreamWriter.hpp"

EditableFont::EditableFont(const String& textureName, const std::unordered_map<wchar_t, FontChar>& charset, float charHeight)
: textureName(textureName), charset(charset), charHeight(charHeight)
{
}

String EditableFont::GetTextureName() const
{
	return textureName;
}

void EditableFont::SetTextureName(const String& textureName)
{
	this->textureName = textureName;
}

std::unordered_map<wchar_t, FontChar>& EditableFont::GetCharset()
{
	return charset;
}

float EditableFont::GetCharHeight() const
{
	return charHeight;
}

void EditableFont::SetCharHeight(float charHeight)
{
	this->charHeight = charHeight;
}

void EditableFont::Serialize(ptr<OutputStream> outputStream)
{
	ptr<Inanity::StreamWriter> writer = NEW(StreamWriter(outputStream));

	// записать заголовок файла
	writer->Write(textureName);
	writer->Write<float>(charHeight);
	writer->WriteShortly(charset.size());

	// записать символы
	for(std::unordered_map<wchar_t, FontChar>::const_iterator i = charset.begin(); i != charset.end(); ++i)
	{
		writer->Write(i->first);
		writer->Write(i->second);
	}
}
