#include "NxFileSystem.hpp"
#include "../MemoryFile.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"
#include "../Exception.hpp"
#include <nn/fs.h>

BEGIN_INANITY_PLATFORM

/// Check result of operation.
/** If message is null, simply return result. Otherwise throw an exception. */
static bool NxCheckResult(const nn::Result& result, const char* message = nullptr)
{
	if(result.IsSuccess()) return true;

	if(!message) return false;

	ptr<Exception> exception;
	if(nn::fs::ResultPathNotFound::Includes(result))
	{
		exception = NEW(Exception("Path not found"));
	}
	else if(nn::fs::ResultPathAlreadyExists::Includes(result))
	{
		exception = NEW(Exception("Path already exists"));
	}
	else if(nn::fs::ResultTargetLocked::Includes(result))
	{
		exception = NEW(Exception("Target locked"));
	}
	else if(nn::fs::ResultUsableSpaceNotEnough::Includes(result))
	{
		exception = NEW(Exception("Usable space not enough"));
	}

	if(exception)
		THROW_SECONDARY(message, exception);
	else
		THROW(message);
}

class NxFileHandle
{
private:
	nn::fs::FileHandle fileHandle = { nullptr };

public:
	NxFileHandle() {}

	NxFileHandle(nn::fs::FileHandle fileHandle)
	: fileHandle(fileHandle) {}

	NxFileHandle(const NxFileHandle&) = delete;

	NxFileHandle(NxFileHandle&& fileHandle)
	: fileHandle(fileHandle.fileHandle)
	{
		fileHandle.fileHandle = { nullptr };
	}

	~NxFileHandle()
	{
		if(fileHandle.handle)
			nn::fs::CloseFile(fileHandle);
	}

	operator nn::fs::FileHandle() const
	{
		return fileHandle;
	}

	nn::fs::FileHandle* operator&()
	{
		return &fileHandle;
	}
};

class NxDirectoryHandle
{
private:
	nn::fs::DirectoryHandle directoryHandle = { nullptr };

public:
	NxDirectoryHandle() {}

	NxDirectoryHandle(nn::fs::DirectoryHandle directoryHandle)
	: directoryHandle(directoryHandle) {}

	NxDirectoryHandle(const NxDirectoryHandle&) = delete;

	NxDirectoryHandle(NxDirectoryHandle&& directoryHandle)
	: directoryHandle(directoryHandle.directoryHandle)
	{
		directoryHandle.directoryHandle = { nullptr };
	}

	~NxDirectoryHandle()
	{
		if(directoryHandle.handle)
			nn::fs::CloseDirectory(directoryHandle);
	}

	operator nn::fs::DirectoryHandle() const
	{
		return directoryHandle;
	}

	nn::fs::DirectoryHandle* operator&()
	{
		return &directoryHandle;
	}
};

class NxStorage;

class NxInputStream : public InputStream
{
private:
	ptr<NxStorage> storage;
	bigsize_t offset;
	bigsize_t endOffset;

public:
	NxInputStream(ptr<NxStorage> storage, bigsize_t offset, bigsize_t endOffset)
	: storage(storage), offset(offset), endOffset(endOffset) {}

	size_t Read(void* data, size_t size) override;

	bigsize_t Skip(bigsize_t size) override
	{
		bigsize_t newOffset = offset + size;
		if(newOffset > endOffset) newOffset = endOffset;
		bigsize_t skipped = newOffset - offset;
		offset = newOffset;
		return skipped;
	}

	bool IsAtEnd() const override
	{
		return offset >= endOffset;
	}
};

class NxOutputStream : public OutputStream
{
	NxFileHandle fileHandle;
	int64_t offset = 0;

public:
	NxOutputStream(NxFileHandle&& fileHandle)
	: fileHandle(std::move(fileHandle)) {}

	~NxOutputStream()
	{
		// flush before close
		nn::fs::FlushFile(fileHandle);
	}

	void Write(const void* data, size_t size) override
	{
		NxCheckResult(nn::fs::WriteFile(fileHandle, offset, data, size, nn::fs::WriteOption::MakeValue(0)), "Can't write to Nx output stream");
		offset += size;
	}
};

class NxStorage : public Storage
{
	NxFileHandle fileHandle;
	bigsize_t size;

public:
	NxStorage(NxFileHandle&& fileHandle, bigsize_t size)
	: fileHandle(std::move(fileHandle)), size(size) {}

	bigsize_t GetBigSize() const override
	{
		return size;
	}

	void Read(bigsize_t offset, size_t size, void* data) override
	{
		NxCheckResult(nn::fs::ReadFile(fileHandle, offset, data, size), "Can't read Nx storage");
	}

	ptr<InputStream> GetInputStream(bigsize_t offset, bigsize_t size) override
	{
		return NEW(NxInputStream(this, offset, offset + size));
	}
};

size_t NxInputStream::Read(void* data, size_t size)
{
	bigsize_t end = offset + size;
	if(end > endOffset) end = endOffset;
	size_t read = end - offset;
	storage->Read(offset, read, data);
	offset = end;
	return read;
}

NxFileSystem::NxFileSystem(const String& userFolderName)
: folderName(userFolderName)
{
	if(!folderName.empty() && folderName[folderName.length() - 1] != '/')
		folderName += "/";
}

String NxFileSystem::GetFullName(const String& fileName) const
{
	if(fileName.empty() || fileName[0] != '/')
		THROW("File name should begin with slash");

	return folderName + fileName.substr(1);
}

ptr<File> NxFileSystem::LoadFile(const String& fileName)
{
	// open file
	NxFileHandle fileHandle;
	NxCheckResult(nn::fs::OpenFile(&fileHandle, GetFullName(fileName).c_str(), nn::fs::OpenMode_Read), "Can't open file");

	// get size
	int64_t fileSize;
	NxCheckResult(nn::fs::GetFileSize(&fileSize, fileHandle), "Can't get file size");

	// load data
	ptr<File> file = NEW(MemoryFile((size_t)fileSize));
	NxCheckResult(nn::fs::ReadFile(fileHandle, 0, file->GetData(), file->GetSize()), "Can't read file");

	return file;
}

ptr<File> NxFileSystem::TryLoadFile(const String& fileName)
{
	// open file
	NxFileHandle fileHandle;
	if(!NxCheckResult(nn::fs::OpenFile(&fileHandle, GetFullName(fileName).c_str(), nn::fs::OpenMode_Read))) return nullptr;

	// get size
	int64_t fileSize;
	if(!NxCheckResult(nn::fs::GetFileSize(&fileSize, fileHandle))) return nullptr;

	// load data
	ptr<File> file = NEW(MemoryFile((size_t)fileSize));
	if(!NxCheckResult(nn::fs::ReadFile(fileHandle, 0, file->GetData(), file->GetSize()))) return nullptr;

	return file;
}

ptr<Storage> NxFileSystem::LoadStorage(const String& fileName)
{
	// open file
	NxFileHandle fileHandle;
	NxCheckResult(nn::fs::OpenFile(&fileHandle, GetFullName(fileName).c_str(), nn::fs::OpenMode_Read), "Can't open file");

	// get size
	int64_t fileSize;
	NxCheckResult(nn::fs::GetFileSize(&fileSize, fileHandle), "Can't get file size");

	return NEW(NxStorage(std::move(fileHandle), fileSize));
}

ptr<InputStream> NxFileSystem::LoadStream(const String& fileName)
{
	ptr<Storage> storage = LoadStorage(fileName);

	return storage->GetInputStream(0, storage->GetBigSize());
}

void NxFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	String fullName = GetFullName(fileName);

	// create file if not exist
	nn::fs::CreateFile(fullName.c_str(), 0);

	// open file
	NxFileHandle fileHandle;
	NxCheckResult(nn::fs::OpenFile(&fileHandle, fullName.c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_AllowAppend), "Can't open file");

	// write data
	NxCheckResult(nn::fs::WriteFile(fileHandle, 0, file->GetData(), file->GetSize(), nn::fs::WriteOption::MakeValue(0)), "Can't write file");

	// apparently flushing after writing is mandatory (otherwise it crashes on CloseFile)
	NxCheckResult(nn::fs::FlushFile(fileHandle), "Can't flush file");;
}

ptr<OutputStream> NxFileSystem::SaveStream(const String& fileName)
{
	// open file
	NxFileHandle fileHandle;
	NxCheckResult(nn::fs::OpenFile(&fileHandle, GetFullName(fileName).c_str(), nn::fs::OpenMode_Write | nn::fs::OpenMode_AllowAppend), "Can't open file");

	// create stream
	return NEW(NxOutputStream(std::move(fileHandle)));
}

#if defined(_DEBUG)

time_t NxFileSystem::GetFileMTime(const String& fileName)
{
	nn::fs::FileTimeStamp fileTimeStamp;
	NxCheckResult(nn::fs::GetFileTimeStampForDebug(&fileTimeStamp, GetFullName(fileName).c_str()), "Can't get file timestamp");
	return fileTimeStamp.modify.value;
}

#endif

void NxFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	// open directory
	NxDirectoryHandle directoryHandle;
	NxCheckResult(nn::fs::OpenDirectory(&directoryHandle, GetFullName(directoryName).c_str(), nn::fs::OpenDirectoryMode_All), "Can't open directory");

	// get entry count
	// we get entries in batches anyway, but we don't want to allocate too much for small directories
	int64_t entriesBufferSize;
	NxCheckResult(nn::fs::GetDirectoryEntryCount(&entriesBufferSize, directoryHandle), "Can't get directory entry count");
	if(entriesBufferSize <= 0) return;
	if(entriesBufferSize > 64) entriesBufferSize = 64;

	nn::fs::DirectoryEntry tempEntries[entriesBufferSize]; // VLA - GNU C++ extension
	for(;;)
	{
		int64_t entriesStored;
		NxCheckResult(nn::fs::ReadDirectory(&entriesStored, tempEntries, directoryHandle, entriesBufferSize), "Can't get directory entries");

		if(entriesStored <= 0) break;

		for(int64_t i = 0; i < entriesStored; ++i)
			entries.push_back(tempEntries[i].name);
	}
}

void NxFileSystem::MakeDirectory(const String& directoryName)
{
	NxCheckResult(nn::fs::CreateDirectory(GetFullName(directoryName).c_str()), "Can't create directory");
}

void NxFileSystem::DeleteEntry(const String& entryName)
{
	// support only deleting files for now
	NxCheckResult(nn::fs::DeleteFile(GetFullName(entryName).c_str()), "Can't delete file");
}

FileSystem::EntryType NxFileSystem::GetEntryType(const String& entryName) const
{
	nn::fs::DirectoryEntryType entryType;
	NxCheckResult(nn::fs::GetEntryType(&entryType, GetFullName(entryName).c_str()), "Can't get entry type");
	switch(entryType)
	{
	case nn::fs::DirectoryEntryType_File:
		return entryTypeFile;
	case nn::fs::DirectoryEntryType_Directory:
		return entryTypeDirectory;
	default:
		return entryTypeOther;
	}
}

ptr<FileSystem> NxFileSystem::GetSubFileSystem(const String& folderName)
{
	return NEW(NxFileSystem(GetFullName(folderName)));
}

END_INANITY_PLATFORM
