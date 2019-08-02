#ifndef ___INANITY_DATA_BLOB_FILE_SYSTEM_HPP___
#define ___INANITY_DATA_BLOB_FILE_SYSTEM_HPP___

#include "data.hpp"
#include "../FileSystem.hpp"
#include <unordered_map>

BEGIN_INANITY

class Storage;

END_INANITY

BEGIN_INANITY_DATA

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
private:
	ptr<Storage> storage;
	typedef std::unordered_map<String, std::pair<bigsize_t, size_t> > Files;
	Files files;

public:
	/// Структура, которой заканчивается файл системы.
	struct Terminator
	{
		static const char magicValue[4];
		/// Сигнатура.
		char magic[4];
		/// Размер заголовка в low endian.
		uint8_t headerSize[4];
	};

	/// Load blob file system from storage.
	BlobFileSystem(ptr<Storage> storage);

	// FileSystem's methods.
	ptr<File> TryLoadFile(const String& fileName) override;

	META_DECLARE_CLASS(BlobFileSystem);
};

END_INANITY_DATA

#endif
