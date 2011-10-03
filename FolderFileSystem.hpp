#ifndef ___INANITY_FOLDER_FILE_SYSTEM_HPP___
#define ___INANITY_FOLDER_FILE_SYSTEM_HPP___

#include "FileSystem.hpp"

BEGIN_INANITY

/// Класс "папковой" файловой системы.
/** Предоставляет доступ к каталогу родной дисковой файловой системы
компьютера, как к отдельной файловой системе.
Как и все файловые системы, использует прямой слеш / как разделитель каталогов в имени.
*/
class FolderFileSystem : public FileSystem
{
private:
	/// Полное Unicode-имя каталога в Windows
	/** Оно абсолютное, вида \\?\C:\..., не оканчивается на слеш. */
	String folderName;

	String GetFullName(String fileName) const;
	void GetFileNames(String sourceDirectory, const String& targetDirectory, std::vector<String>& fileNames) const;

	/// Конструктор для создания абсолютной файловой системы.
	FolderFileSystem();

public:
	/// Создать "папковую" файловую систему.
	/** Создает объект файловой системы, относящийся к заданному
	каталогу. В качестве каталога можно указать строку нулевой длины,
	в таком случае считается, что объект файловой системы
	относится к текущему каталогу приложения.
	\param folderName Имя каталога файловой системы, или строка
	нулевой длины. Каталог задаётся относительно текущего каталога,
	кроме случая, когда folderName[1]==':', тогда он считается абсолютным.
	*/
	FolderFileSystem(const String& userFolderName);

	/// Создать абсолютную "папковую" файловую систему.
	/** Такая файловая система не производит никаких преобразований с
	именами файлов, и поэтому может принимать и абсолютные пути.
	Но зато она не может возвращать список файлов. */
	static ptr<FolderFileSystem> GetNativeFileSystem();

	//методы, унаследованные из FileSystem
	ptr<File> LoadFile(const String& fileName);
	ptr<InputStream> LoadFileAsStream(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	ptr<OutputStream> SaveFileAsStream(const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;

	//расширенные методы для загрузки файлов, специфичные для FolderFileSystem

	/// Получить размер файла.
	/**
	\param fileName Имя файла.
	\return Размер файла в байтах.
	*/
	size_t GetFileSize(const String& fileName);
	/// Загрузить часть файла.
	/** Самый мощный метод для загрузки файлов. Остальные методы работают
	через него. Позволяет задать начало и размер проекции файла в память,
	чтобы проецировать только нужную часть файла.
	\param fileName Имя файла.
	\param mappingStart Требуемое смещение до начала проекции файла. Допускается
	указывать значения, не кратные гранулярности выделения памяти.
	\param mappingSize Требуемый размер проекции файла. Допускается указывать
	значения, не кратные гранулярости выделения памяти. Значение 0 означает,
	что проекция файла занимать весь объем файла.
	\return Объект-файл.
	*/
	ptr<File> LoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize);
};

END_INANITY

#endif
