#ifndef ___INANITY_STRINGS_HPP___
#define ___INANITY_STRINGS_HPP___

#include "String.hpp"

/*
Файл содержит статический класс, предлагающий некоторые сервисы работы
со строками.
*/

BEGIN_INANITY

class File;
class Strings
{
public:
	/// Преобразовать строку из Unicode в UTF-8.
	static std::string Unicode2UTF8(const std::wstring& str);
	/// Преобразовать строку из UTF-8 в Unicode.
	static std::wstring UTF82Unicode(const std::string& str);

	/// Преобразовать строку в файл.
	static ptr<File> String2File(const std::string& str);
	/// Преобразовать файл в строку.
	static std::string File2String(ptr<File> file);

	/// Преобразовать int в строку.
	static String ToString(int number);
	/// Преобразовать данные в hex.
	static String ToHex(const void* data, size_t size);
};

END_INANITY

#endif
