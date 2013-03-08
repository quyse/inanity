#ifndef ___INANITY_FILTER_FILE_SYSTEM_HPP___
#define ___INANITY_FILTER_FILE_SYSTEM_HPP___

#include "FileSystem.hpp"

BEGIN_INANITY

/// Класс фильтрующей файловой системы.
/** Просто переадресует все запросы подлежащей файловой системе.
Полезен для реализации на его основе других фильтров файловых систем. */
class FilterFileSystem : public FileSystem
{
protected:
	ptr<FileSystem> fileSystem;

public:
	FilterFileSystem(ptr<FileSystem> fileSystem);

	ptr<File> LoadFile(const String& fileName);
	ptr<File> TryLoadFile(const String& fileName);
	ptr<InputStream> LoadStream(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	ptr<OutputStream> SaveStream(const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;
	void GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;
	void GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;
};

END_INANITY

#endif
