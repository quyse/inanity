#ifndef ___INANITY_BLOB_FILE_SYSTEM_HPP___
#define ___INANITY_BLOB_FILE_SYSTEM_HPP___

#include "FileSystem.hpp"
#include "meta/decl.hpp"
#include <unordered_map>

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
		static const char magicValue[4];
		/// Сигнатура.
		char magic[4];
		/// Размер заголовка.
		unsigned headerSize;
	};

private:
	/// Исходный файл.
	ptr<File> file;
	/// Хеш файлов из файловой системы.
	std::unordered_map<String, ptr<File> > files;

public:
	//открыть существующую файловую систему
	BlobFileSystem(ptr<File> file);

	ptr<File> TryLoadFile(const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;
	bool IsFileExists(const String& fileName) const;

	META_DECLARE_CLASS(BlobFileSystem);
};

END_INANITY

#endif
