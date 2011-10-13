#ifndef ___INANITY_STRINGS_HPP___
#define ___INANITY_STRINGS_HPP___

#include "Object.hpp"
#include <string>

/*
Файл содержит статический класс, предлагающий некоторые сервисы работы
со строками.
*/

BEGIN_INANITY

class File;
class Strings
{
public:
	/// Преобразовать строку из ASCII в Unicode.
	static std::wstring Ascii2Unicode(const std::string& str);

	/// Преобразовать строку из Unicode в UTF-8.
	static std::string Unicode2UTF8(const std::wstring& str);
	/// Преобразовать строку из UTF-8 в Unicode.
	static std::wstring UTF82Unicode(const std::string& str);

	/// Преобразовать строку ASCII в файл.
	static ptr<File> Ascii2File(const std::string& str);
	/// Преобразовать файл в строку ASCII.
	static std::string File2Ascii(ptr<File> file);
};

END_INANITY

#endif