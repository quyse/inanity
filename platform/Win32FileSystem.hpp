#ifndef ___INANITY_PLATFORM_WIN32_FILE_SYSTEM_HPP___
#define ___INANITY_PLATFORM_WIN32_FILE_SYSTEM_HPP___

#include "platform.hpp"
#include "../FileSystem.hpp"

BEGIN_INANITY

class Exception;

END_INANITY

BEGIN_INANITY_PLATFORM

/// Windows file system.
/** Uses native Windows API. */
class Win32FileSystem : public FileSystem
{
private:
	/// Full directory name.
	/** It's absolute (like \\?\C:\... в Windows), and without '/' at end. */
	String folderName;

	String GetFullName(String fileName) const;

	/// Create absolute file system.
	Win32FileSystem();

public:
	/// Creates Win32 file system.
	/** Creates file system represents specified directory.
	If empty string is specified as a directory, it works
	for current directory.
	Directory is specified relative to current directory
	except if userFolderName[1] == ':', then it's absolute.
	*/
	Win32FileSystem(const String& userFolderName);

	/// Create native file system.
	/** Such system works with native platform file names. */
	static ptr<Win32FileSystem> GetNativeFileSystem();

	//*** FileSystem's methods.
	ptr<File> LoadFile(const String& fileName);
	ptr<File> TryLoadFile(const String& fileName);
	ptr<InputStream> LoadStream(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	ptr<OutputStream> SaveStream(const String& fileName);
	time_t GetFileMTime(const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;
	void GetDirectoryEntries(const String& directoryName, std::vector<String>& fileNames) const;
	void MakeDirectory(const String& directoryName);
	void DeleteEntry(const String& entryName);
	EntryType GetEntryType(const String& entryName) const;

	//*** Extended methods for Win32FileSystem.

	/// Get file size.
	size_t GetFileSize(const String& fileName);
	/// Load part of file.
	/** Mapping start and mapping don't have to be a multiple
	of system allocation granularity.
	If mappingSize == 0 then whole size of file will be used.
	*/
	ptr<File> TryLoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize, ptr<Exception>* exception);

	META_DECLARE_CLASS(Win32FileSystem);
};

END_INANITY_PLATFORM

#endif
