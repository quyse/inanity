#ifndef ___INANITY_DATA_COMPOSITE_FILE_SYSTEM_H___
#define ___INANITY_DATA_COMPOSITE_FILE_SYSTEM_H___

#include "data.hpp"
#include "../FileSystem.hpp"
#include <vector>

BEGIN_INANITY_DATA

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

	/// Check that file system accepts that file.
	/** Returns non-empty subpath for that file system if it does. */
	String TryFileSystemForPath(size_t i, const String& path);

public:
	/// Примонтировать файловую систему в указанный путь.
	void Mount(ptr<FileSystem> fileSystem, String path = "/");

	ptr<File> TryLoadFile(const String& fileName) override;
};

END_INANITY_DATA

#endif
