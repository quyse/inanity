#ifndef ___INANITY_DATA_BLOB_FILE_SYSTEM_HPP___
#define ___INANITY_DATA_BLOB_FILE_SYSTEM_HPP___

#include "data.hpp"
#include "../meta/decl.hpp"
#include <cstdint>

BEGIN_INANITY

class File;
class FileSystem;

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
class BlobFileSystem
{
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

	/// Открыть и распаковать blob-файловую систему в заданную файловую систему.
	static void Unpack(ptr<File> file, ptr<FileSystem> fileSystem);

	/// Загрузить blob-файловую систему.
	static ptr<FileSystem> Load(ptr<File> file);

	META_DECLARE_CLASS(BlobFileSystem);
};

END_INANITY_DATA

#endif
