#include "Strings.hpp"
#include "MemoryFile.hpp"
#include "utf8.h"
#include <cstring>

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
	_itoa(number, str, 10);
	return str;
}
