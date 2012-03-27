#include "EditableFont.hpp"
#include "../OutputStream.hpp"
#include "../StreamWriter.hpp"

EditableFont::EditableFont(const String& textureName, const Charset& charset, const KerningPairs& kerningPairs, float charHeight)
: textureName(textureName), charset(charset), kerningPairs(kerningPairs), charHeight(charHeight)
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

EditableFont::Charset& EditableFont::GetCharset()
{
	return charset;
}

EditableFont::KerningPairs& EditableFont::GetKerningPairs()
{
	return kerningPairs;
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

	// записать символы
	writer->WriteShortly(charset.size());
	for(Charset::const_iterator i = charset.begin(); i != charset.end(); ++i)
	{
		writer->Write(i->first);
		writer->Write(i->second);
	}

	// записать кернинг-пары
	for(KerningPairs::const_iterator i = kerningPairs.begin(); i != kerningPairs.end(); ++i)
	{
		writer->Write(i->first.first);
		writer->Write(i->first.second);
		writer->Write(i->second);
	}
}
