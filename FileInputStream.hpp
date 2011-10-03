#ifndef ___INANITY_FILE_INPUT_STREAM_HPP___
#define ___INANITY_FILE_INPUT_STREAM_HPP___

#include "InputStream.hpp"

BEGIN_INANITY

class File;
/// Класс потока ввода, считывающий из любого файла (объекта File).
class FileInputStream : public InputStream
{
private:
	ptr<File> file;
	const char* fileData;
	size_t fileSize;

public:
	/// создает объект FileInputStream с указанным файлом
	/** \param file объект File для чтения */
	FileInputStream(ptr<File> file);

	size_t Read(void* data, size_t size);
};

END_INANITY

#endif
