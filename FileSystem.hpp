#ifndef ___INANITY_FILE_SYSTEM_HPP___
#define ___INANITY_FILE_SYSTEM_HPP___

#include "Object.hpp"
#include "String.hpp"
#include "meta/decl.hpp"
#include <vector>
#include <ctime>

BEGIN_INANITY

/*
Файл содержит абстрактный класс файловой системы.
Методы LoadFile и TryLoadFile отличаются только тем, что
TryLoadFile может выдавать нулевую ссылку (при отсутствии файла),
а LoadFile бросает в таком случае исключение.
*/

class File;
class InputStream;
class OutputStream;
/// Абстрактный класс файловой системы.
/** Файловая система - это набор файлов, к которым можно обращаться по именам.
Файловая система не гарантирует постоянство этого набора.
Для реализации необходимо определить хотя бы один из методов LoadFile или TryLoadFile.
В случае неопределенности одного из методов, реализация по умолчанию реализует его
через другой метод. Можно реализовывать оба метода, если это более эффективно в данной
файловой системе. Реализация TryLoadFile по умолчанию работает с некоторой потерей в
производительности (за счет перехвата исключений из LoadFile). Реализация LoadFile
вполне эффективна (так как перехват исключений не требуется). Поэтому рекомендуется
реализовывать хотя бы TryLoadFile.
Для реализации также можно определить методы LoadStream и SaveStream.
Реализация LoadStream по умолчанию использует метод LoadFile вместе с классом
MemoryStream. Реализация SaveStream только бросает исключение.
В качестве разделителя каталогов используется прямой слеш /.
Путь должен начинаться со слеша.
Имя каталога должно заканчиваться слешем.
*/
class FileSystem : public Object
{
public:
	enum EntryType
	{
		entryTypeMissing,
		entryTypeFile,
		entryTypeDirectory,
		entryTypeOther
	};

	/// Загрузить файл.
	/** Возвращает объект File для заданного файла. При отсутствии файла
	с заданным именем выбрасывает исключение.
	\param fileName Имя файла в данной файловой системе.
	\return Объект File для заданного файла.
	*/
	virtual ptr<File> LoadFile(const String& fileName);

	/// Загрузить файл.
	/** Возвращает объект File для заданного файла. При отсутствии файла
	с заданным именем возвращает null.
	\param fileName Имя файла в данной файловой системе.
	\return Объект File для заданного файла, или null, если файл не
	удалось загрузить.
	*/
	virtual ptr<File> TryLoadFile(const String& fileName);

	/// Открыть файл как поток ввода.
	/** Возвращает поток ввода, предназначенный для чтения файла.
	\param fileName Имя файла.
	\return Поток ввода.
	*/
	virtual ptr<InputStream> LoadStream(const String& fileName);

	/// Сохранить файл.
	/** Сохраняет файл в файловой системе, с заданным именем. Если файловая
	система не поддерживает сохранение файлов, выбрасывается исключение.
	\param file Файл, то есть данные, которые необходимо сохранить.
	\param fileName Имя в файловой системе, под которым сохраняются данные.
	*/
	virtual void SaveFile(ptr<File> file, const String& fileName);

	/// Открыть файл как поток вывода.
	/** Возвращает поток вывода, предназначенный для записи в файл.
	\param fileName Имя файла.
	\return Поток вывода.
	*/
	virtual ptr<OutputStream> SaveStream(const String& fileName);

	/// Получить время модификации файла.
	virtual time_t GetFileMTime(const String& fileName);

	/// Получить список имен файлов в файловой системе.
	/** Получает список имен файлов. Не гарантируется, что он будет полным
	и точным. Если файловая система не поддерживает получение списка
	имен файлов, выбрасывается исключение.
	\param fileNames Выходной список имен файлов.
	*/
	virtual void GetFileNames(std::vector<String>& fileNames) const;

	/// Получить список файлов и каталогов в заданном каталоге (нерекурсивно).
	/** Получает список имён файлов и каталогов в заданном каталоге.
	Если файловая система не поддерживает это, выбрасывается исключение.
	\param directoryName Имя каталога, оканчивающееся слешем.
	\param entries Выходной список с относительными именами файлов и каталогов.
	*/
	virtual void GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;

	/// Получить список всех файлов и каталогов, начиная с данного каталога (рекурсивно).
	/**
	\param directoryName Имя каталога, оканчивающееся слешем.
	\param entries Выходной список с абсолютными именами файлов и каталогов.
	*/
	virtual void GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;

	/// Create directory.
	/**
	\param directoryName Directory name with slash at the end.
	*/
	virtual void MakeDirectory(const String& directoryName);

	/// Delete file or directory.
	/**
	\param entryName Entry name to delete.
	*/
	virtual void DeleteEntry(const String& entryName);

	/// Get entry type.
	/**
	\param entryName File or directory name.
	*/
	virtual EntryType GetEntryType(const String& entryName) const;

	/// Create sub file system.
	virtual ptr<FileSystem> GetSubFileSystem(const String& folderName);

	META_DECLARE_CLASS(FileSystem);
};

END_INANITY

#endif
