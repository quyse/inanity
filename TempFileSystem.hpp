#ifndef ___INANITY_TEMP_FILE_SYSTEM_HPP___
#define ___INANITY_TEMP_FILE_SYSTEM_HPP___

#include "FileSystem.hpp"
#include <unordered_map>

BEGIN_INANITY

class ResourceLoader;

/// Класс временной файловой системы.
/** Просто хеш с файлами. Позволяет добавлять файлы во время работы. */
class TempFileSystem : public FileSystem
{
private:
	std::unordered_map<String, ptr<File> > files;

public:
	ptr<File> TryLoadFile(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
};

END_INANITY

#endif
