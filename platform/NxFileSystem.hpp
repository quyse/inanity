#ifndef ___INANITY_PLATFORM_NX_FILE_SYSTEM_HPP___
#define ___INANITY_PLATFORM_NX_FILE_SYSTEM_HPP___

#include "platform.hpp"
#include "../FileSystem.hpp"

BEGIN_INANITY_PLATFORM

/// Nintendo Switch file system.
/** Uses nn::fs API. */
class NxFileSystem : public FileSystem
{
private:
	/// Full directory name.
	/** It's absolute, and with '/' at end. */
	String folderName;

	String GetFullName(const String& fileName) const;

public:
	/// Create NxFileSystem from absolute directory name.
	NxFileSystem(const String& userFolderName);

	// FileSystem's methods.
	ptr<File> LoadFile(const String& fileName);
	ptr<InputStream> LoadStream(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	ptr<OutputStream> SaveStream(const String& fileName);
#if defined(_DEBUG)
	time_t GetFileMTime(const String& fileName);
#endif
	void GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;
	void MakeDirectory(const String& directoryName);
	void DeleteEntry(const String& entryName);
	EntryType GetEntryType(const String& entryName) const;
	ptr<FileSystem> GetSubFileSystem(const String& folderName);
};

END_INANITY_PLATFORM

#endif
