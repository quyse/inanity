#ifndef ___INANITY_BLOB_FILE_SYSTEM_HPP___
#define ___INANITY_BLOB_FILE_SYSTEM_HPP___

#include "FileSystem.hpp"
#include <hash_map>

BEGIN_INANITY

/// Класс blob-файловой системы
/** То есть системы, хранящей архив в одном файле.
 * Формат такой: сначала как-то идут данные.
 * В конце файла находится заголовок и структура-терминатор.
 * Заголовок - это последовательность имён файлов со смещениями.
 *
 * Формат с заголовком в конце выбран специально, чтобы упростить
 * формирование такой системы на лету, не в памяти.
 * */
class BlobFileSystem : public FileSystem
{
public:
	/// Структура, которой заканчивается файл системы.
	struct Terminator
	{
		static const unsigned signatureValue = 'BOLB';
		/// Сигнатура.
		unsigned signature;
		/// Размер заголовка.
		unsigned headerSize;
	};

private:
	/// Исходный файл.
	ptr<File> file;
	/// Хеш файлов из файловой системы.
	std::hash_map<String, ptr<File> > files;

public:
	//открыть существующую файловую систему
	BlobFileSystem(ptr<File> file);

	ptr<File> TryLoadFile(const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;
	bool IsFileExists(const String& fileName) const;
};

END_INANITY

#endif
