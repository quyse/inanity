#include "Strings.hpp"
#include "MemoryFile.hpp"
#include <cstring>

#ifdef ___INANITY_WINDOWS

#include "windows.hpp"

std::wstring Strings::Ascii2Unicode(const std::string& str)
{
	std::wstring res(MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), NULL, 0), L' ');
	if(res.length())
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), &*res.begin(), (int)res.length());
	return res;
}

std::string Strings::Unicode2UTF8(const std::wstring& str)
{
	std::string res(WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0, nullptr, nullptr), ' ');
	if(res.length())
		WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), &*res.begin(), (int)res.length(), nullptr, nullptr);
	return res;
}

std::wstring Strings::UTF82Unicode(const std::string& str)
{
	std::wstring res(MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0), L' ');
	if(res.length())
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &*res.begin(), (int)res.length());
	return res;
}

#endif

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
