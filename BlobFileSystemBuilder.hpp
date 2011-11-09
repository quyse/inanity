#ifndef ___INANITY_BLOB_FILE_SYSTEM_BUILDER_HPP___
#define ___INANITY_BLOB_FILE_SYSTEM_BUILDER_HPP___

#include "String.hpp"

BEGIN_INANITY

class OutputStream;
class MemoryStream;
class InputStream;
class StreamWriter;
class File;

/// Класс построителя файловой системы.
class BlobFileSystemBuilder : public Object
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

	/// Добавить файл.
	/**
		\param alignment Выравнивание файла в потоке данных.
	*/
	void AddFile(const String& fileName, ptr<File> file, size_t alignment = 4);
	/// Добавить поток в качестве файла.
	/**
		\param alignment Выравнивание файла в потоке данных.
	*/
	void AddFileStream(const String& fileName, ptr<InputStream> fileStream, size_t alignment = 4);

	/// Завершить создание файловой системы.
	void Finalize();
};

END_INANITY

#endif
