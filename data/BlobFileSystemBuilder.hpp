#ifndef ___INANITY_DATA_BLOB_FILE_SYSTEM_BUILDER_HPP___
#define ___INANITY_DATA_BLOB_FILE_SYSTEM_BUILDER_HPP___

#include "data.hpp"
#include "../FileSystem.hpp"
#include "../meta/decl.hpp"

BEGIN_INANITY

class MemoryStream;
class StreamWriter;

END_INANITY

BEGIN_INANITY_DATA

/// Класс построителя файловой системы.
/** Для удобства также является write-only файловой системой. */
class BlobFileSystemBuilder : public FileSystem
{
private:
	/// Записыватель, выполняющий запись в файловую систему.
	ptr<StreamWriter> outputWriter;
	/// Поток, в котором формируется заголовок.
	ptr<MemoryStream> headerStream;
	/// Записыватель заголовка.
	ptr<StreamWriter> headerWriter;

public:
	/// Создать построитель файловой системы.
	/** Указывается поток, в который следует записывать данные. */
	BlobFileSystemBuilder(ptr<OutputStream> outputStream);

	/// Методы FileSystem.
	ptr<File> TryLoadFile(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);

	/// Добавить файл.
	/**
		\param alignment Выравнивание файла в потоке данных.
	*/
	void AddFile(const String& fileName, ptr<File> file, size_t alignment = 16);
	/// Добавить поток в качестве файла.
	/**
		\param alignment Выравнивание файла в потоке данных.
	*/
	void AddFileStream(const String& fileName, ptr<InputStream> fileStream, size_t alignment = 16);

	/// Завершить создание файловой системы.
	void Finalize();

	META_DECLARE_CLASS(BlobFileSystemBuilder);
};

END_INANITY_DATA

#endif
