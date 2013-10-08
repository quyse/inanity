#ifndef ___INANITY_PLATFORM_POSIX_FILE_SYSTEM_HPP___
#define ___INANITY_PLATFORM_POSIX_FILE_SYSTEM_HPP___

#include "platform.hpp"
#include "../FileSystem.hpp"

BEGIN_INANITY_PLATFORM

/// Posix file system.
/** Uses Posix API. */
class PosixFileSystem : public FileSystem
{
private:
	/// Full directory name.
	/** It's absolute, and without '/' at end. */
	String folderName;

	String GetFullName(String fileName) const;

	/// Create absolute file system.
	PosixFileSystem();

public:
	/// Creates Posix file system.
	/** Creates file system represents specified directory.
	If empty string is specified as a directory, it works
	for current directory.
	Path can be relative or absolute.
	*/
	PosixFileSystem(const String& userFolderName);

	/// Create native file system.
	/** Such system works with native platform file names. */
	static ptr<PosixFileSystem> GetNativeFileSystem();

	//*** FileSystem's methods.
	ptr<File> LoadFile(const String& fileName);
	ptr<InputStream> LoadStream(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	ptr<OutputStream> SaveStream(const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;
	void GetDirectoryEntries(const String& directoryName, std::vector<String>& fileNames) const;

	//*** Extended methods for PosixFileSystem.

	/// Get file size.
	size_t GetFileSize(const String& fileName);
	/// Load part of file.
	/** Mapping start and mapping don't have to be a multiple
	of system allocation granularity.
	If mappingSize == 0 then whole size of file will be used.
	*/
	ptr<File> LoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize);

	META_DECLARE_CLASS(PosixFileSystem);
};

END_INANITY_PLATFORM

#endif
