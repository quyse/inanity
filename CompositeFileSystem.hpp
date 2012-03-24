#ifndef ___INANITY_COMPOSITE_FILE_SYSTEM_H___
#define ___INANITY_COMPOSITE_FILE_SYSTEM_H___

#include "FileSystem.hpp"
#include <vector>

BEGIN_INANITY

class File;

/// Класс составной файловой системы.
/** Позволяет объединять несколько файловых систем.
Файловые системы можно монтировать в различные каталоги, указывая префикс пути,
который должен быть у имени файла.
Файл открывается той файловой системой, имеющей подходящий префикс.
Если таких несколько, файл открывается той системой, которая выше по списку. */
class CompositeFileSystem : public FileSystem
{
private:
	/// Список файловых систем с путями монтирования.
	/** Путь монтирования должен начинаться и заканчиваться на слеш /. */
	std::vector<std::pair<String, ptr<FileSystem> > > fileSystems;

	/// Получить файловую систему, которая должна работать с данным файлом.
	/** Если файловая система найдена, то отрезает от файла префикс. */
	ptr<FileSystem> GetFileSystemForPath(String& path);

public:
	/// Примонтировать файловую систему в корень.
	void MountFileSystem(ptr<FileSystem> fileSystem);
	/// Примонтировать файловую систему в указанный путь.
	void MountFileSystem(String path, ptr<FileSystem> fileSystem);

	ptr<File> LoadFile(const String& fileName);
	ptr<File> TryLoadFile(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
};

END_INANITY

#endif
