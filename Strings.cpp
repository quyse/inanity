#include "Strings.hpp"
#include "MemoryFile.hpp"
#include "scripting_impl.hpp"
#include "utf8.h"
#include <cstring>

BEGIN_INANITY

SCRIPTABLE_MAP_BEGIN(Strings, Inanity.Strings);
	SCRIPTABLE_METHOD(Strings, String2File);
	SCRIPTABLE_METHOD(Strings, File2String);
SCRIPTABLE_MAP_END();

std::string Strings::Unicode2UTF8(const std::wstring& str)
{
	std::string res;
	utf8::unchecked::utf16to8(str.begin(), str.end(), std::back_inserter(res));
	return res;
}

std::wstring Strings::UTF82Unicode(const std::string& str)
{
	std::wstring res;
	utf8::unchecked::utf8to16(str.begin(), str.end(), std::back_inserter(res));
	return res;
}

ptr<File> Strings::String2File(const std::string& str)
{
	ptr<File> file = NEW(MemoryFile(str.length()));
	memcpy(file->GetData(), str.c_str(), file->GetSize());
	return file;
}

std::string Strings::File2String(ptr<File> file)
{
	char* str = (char*)file->GetData();
	return std::string(str, str + file->GetSize());
}

String Strings::ToString(int number)
{
	char str[11];
	sprintf(str, "%d", number);
	return str;
}

String Strings::ToHex(const void* data, size_t size)
{
	static const char hex[] = "0123456789abcdef";
	String str(size * 2, ' ');
	size_t j = 0;
	for(size_t i = 0; i < size; ++i)
	{
		unsigned char byte = ((const unsigned char*)data)[i];
		str[j++] = hex[byte >> 4];
		str[j++] = hex[byte & 15];
	}
	return str;
}

END_INANITY
