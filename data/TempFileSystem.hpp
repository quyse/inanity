#ifndef ___INANITY_DATA_TEMP_FILE_SYSTEM_HPP___
#define ___INANITY_DATA_TEMP_FILE_SYSTEM_HPP___

#include "data.hpp"
#include "../FileSystem.hpp"
#include <unordered_map>

BEGIN_INANITY_DATA

/// Класс временной файловой системы.
/** Просто хеш с файлами. Позволяет добавлять файлы во время работы. */
class TempFileSystem : public FileSystem
{
private:
	typedef std::unordered_map<String, ptr<File> > Files;
	Files files;

public:
	ptr<File> TryLoadFile(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;
};

END_INANITY_DATA

#endif
