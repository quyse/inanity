#ifndef ___INANITY_SUB_FILE_SYSTEM_HPP___
#define ___INANITY_SUB_FILE_SYSTEM_HPP___

#include "FileSystem.hpp"

BEGIN_INANITY

/// Filesystem which is a subfolder of a given filesystem.
class SubFileSystem : public FileSystem
{
private:
	ptr<FileSystem> fileSystem;
	String path;

	String GetName(const String& fileName) const;

public:
	SubFileSystem(ptr<FileSystem> fileSystem, const String& userPath);

	// FileSystem methods.
	ptr<File> LoadFile(const String& fileName);
	ptr<File> TryLoadFile(const String& fileName);
	ptr<InputStream> LoadStream(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	ptr<OutputStream> SaveStream(const String& fileName);
	time_t GetFileMTime(const String& fileName);
	// void GetFileNames(std::vector<String>& fileNames) const;
	void GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;
	void GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;
	void MakeDirectory(const String& directoryName);
	void DeleteEntry(const String& entryName);
	EntryType GetEntryType(const String& entryName) const;
	ptr<FileSystem> GetSubFileSystem(const String& folderName);
};

END_INANITY

#endif
