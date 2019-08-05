#include "Win32FileSystem.hpp"
#include "Win32Handle.hpp"
#include "../File.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"
#include "../MemoryFile.hpp"
#include "../EmptyFile.hpp"
#include "../PartFile.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include <algorithm>

BEGIN_INANITY_PLATFORM

#if !defined(___INANITY_PLATFORM_XBOX)

class Win32MemoryMappedFile : public File
{
private:
	void* data;
	size_t size;

public:
	Win32MemoryMappedFile(void* data, size_t size)
	: data(data), size(size) {}

	~Win32MemoryMappedFile()
	{
		UnmapViewOfFile(data);
	}

	void* GetData() const
	{
		return data;
	}

	size_t GetSize() const
	{
		return size;
	}
};

#endif

class Win32Storage;

class Win32InputStream : public InputStream
{
private:
	ptr<Win32Storage> storage;
	bigsize_t offset;
	bigsize_t endOffset;
	Win32Handle handle;

public:
	Win32InputStream(ptr<Win32Storage> storage, bigsize_t offset, bigsize_t endOffset)
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
};

class Win32OutputStream : public OutputStream
{
private:
	Win32Handle handle;

public:
	Win32OutputStream(Win32Handle&& handle)
	: handle(std::move(handle)) {}

	void Write(const void* data, size_t size)
	{
		if((DWORD)size != size)
			THROW("So big write size is not supported");
		DWORD written;
		if(!WriteFile(handle, data, (DWORD)size, &written, NULL) || written != size)
			THROW("Disk write error");
	}
};

class Win32Storage : public Storage
{
private:
	Win32Handle handle;
	bigsize_t totalSize;
	bigsize_t currentOffset;

public:
	Win32Storage(Win32Handle&& handle, bigsize_t totalSize)
	: handle(std::move(handle)), totalSize(totalSize), currentOffset(0) {}

	bigsize_t GetBigSize() const override
	{
		return totalSize;
	}

	void Read(bigsize_t offset, size_t size, void* data) override
	{
		if((DWORD)size != size)
			THROW("So big reading size is not supported");

		if(offset != currentOffset)
		{
			LARGE_INTEGER newOffset;
			newOffset.QuadPart = (LONGLONG)offset;
			if(!SetFilePointerEx(handle, newOffset, nullptr, FILE_BEGIN))
				THROW("Can't move file pointer");
			currentOffset = offset;
		}

		DWORD read;
		BOOL ok = ReadFile(handle, data, (DWORD)size, &read, NULL);
		if(ok)
			currentOffset += read;
		if(!ok || read != size)
			THROW("Can't read data");
	}

	ptr<InputStream> GetInputStream(bigsize_t offset, bigsize_t size) override
	{
		return NEW(Win32InputStream(this, offset, offset + size));
	}
};

size_t Win32InputStream::Read(void* data, size_t size)
{
	bigsize_t end = offset + size;
	if(end > endOffset) end = endOffset;
	size_t read = (size_t)(end - offset);
	storage->Read(offset, read, data);
	offset = end;
	return read;
}

Win32FileSystem::Win32FileSystem(const String& userFolderName)
{
	//если имя абсолютное
	if(userFolderName.length() >= 2 && userFolderName[1] == ':')
		folderName = userFolderName;
	//иначе относительное
	else
	{
		//получить полное имя текущего каталога
		DWORD length = GetCurrentDirectory(0, NULL);
		wchar_t* buffer = new wchar_t[length];
		if(!GetCurrentDirectory(length, buffer))
		{
			delete [] buffer;
			THROW("Can't get current directory");
		}
		folderName = Strings::Unicode2UTF8(buffer);
		delete [] buffer;

		//прибавить к нему заданное имя каталога, и получить таким образом полный каталог
		if(userFolderName.length())
		{
			folderName += '\\';
			folderName += userFolderName;
		}
	}

	//заменить все слеши на обратные
	for(char& i : folderName)
		if(i == '/') i = '\\';

	//убедиться, что есть слеш в конце
	if(!folderName.empty() && folderName[folderName.length() - 1] != '\\')
		folderName += '\\';

	//убедиться, что имя каталога поддерживает длинные имена
	if(folderName.compare(0, 4, "\\\\?\\") != 0)
		folderName = "\\\\?\\" + folderName;
}

Win32FileSystem::Win32FileSystem()
{
	//создать абсолютную файловую систему, то есть ничего не делать;
	//folderName - пустая строка
}

String Win32FileSystem::GetFullName(String fileName) const
{
	if(!folderName.empty() && !fileName.empty() && fileName[0] == '/')
		fileName = fileName.substr(1);
	String result = folderName.empty() ? fileName : (folderName + fileName);
	size_t length = result.length();
	for(size_t i = 0; i < length; ++i)
		if(result[i] == '/')
			result[i] = '\\';
	return result;
}

size_t Win32FileSystem::GetFileSize(const String& fileName)
{
	Win32Handle hFile = CreateFile(Strings::UTF82Unicode(GetFullName(fileName)).c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(!hFile.IsValid())
		THROW("Can't open file to determine file size");

	LARGE_INTEGER size;
	if(!::GetFileSizeEx(hFile, &size))
		THROW("Can't get file size");

	size_t resultSize = (size_t)size.QuadPart;
	if(resultSize != size.QuadPart)
		THROW("File is too big");
	
	return resultSize;
}

ptr<File> Win32FileSystem::TryLoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize, ptr<Exception>* exception)
{
	String name = GetFullName(fileName);
	do
	{
		// open file
		Win32Handle hFile = CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);
		if(!hFile.IsValid())
		{
			if(exception)
				*exception = NEW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
			break;
		}
		// determine mapping size if not known
		size_t size;
		if(mappingSize)
			size = mappingSize;
		else
		{
			LARGE_INTEGER li;
			::GetFileSizeEx(hFile, &li);
			size = (size_t)li.QuadPart;
			if(size != li.QuadPart)
			{
				if(exception)
					*exception = NEW_EXCEPTION("File too long to map");
				break;
			}
		}
		// shortcut for empty mapping
		if(!size)
			return NEW(EmptyFile());

		// Xbox doesn't support file mappings
#if defined(___INANITY_PLATFORM_XBOX)

		ptr<MemoryFile> file = NEW(MemoryFile(size));
		// support only zero mapping start for now
		if(mappingStart)
		{
			if(exception)
				*exception = NEW_EXCEPTION("Only zero mapping start supported");
			break;
		}
		// read data
		DWORD read;
		ReadFile(hFile, file->GetData(), (DWORD)size, &read, NULL);
		if(read != size)
		{
			if(exception)
				*exception = NEW_EXCEPTION("Can't read data");
			break;
		}
		return file;

#else

		Win32Handle hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		if(!hMapping)
		{
			if(exception)
				*exception = NEW_SECONDARY_EXCEPTION("Can't create file mapping", Exception::SystemError());
			break;
		}

		//получить гранулярность выделения памяти
		static unsigned allocationGranularity = 0;
		if(!allocationGranularity)
		{
			SYSTEM_INFO systemInfo;
			GetSystemInfo(&systemInfo);
			allocationGranularity = systemInfo.dwAllocationGranularity;
		}

		//округлить начало проекции вниз на гранулярность
		long long realMappingStart = mappingStart & ~(long long)(allocationGranularity - 1);
		//вычислить реальный размер
		size_t realMappingSize = mappingSize + (size_t)(mappingStart - realMappingStart);
		//спроецировать файл с учетом этого сдвига
		void* data = MapViewOfFile(hMapping, FILE_MAP_READ, realMappingStart >> 32, realMappingStart & ((1LL << 32) - 1), realMappingSize);
		if(!data)
		{
			if(exception)
				*exception = NEW_SECONDARY_EXCEPTION("Can't map view of file", Exception::SystemError());
			break;
		}

		//если сдвиг был
		if(realMappingStart < mappingStart)
			//вернуть указатель на частичный файл, с учетом сдвига
			return NEW(PartFile(NEW(Win32MemoryMappedFile(data, realMappingSize)), (char*)data + (size_t)(mappingStart - realMappingStart), size));
		//иначе сдвига не было, просто вернуть файл
		return NEW(Win32MemoryMappedFile(data, size));

#endif
	}
	while(false);

	if(exception)
		*exception = NEW_SECONDARY_EXCEPTION("Can't load file \"" + fileName + "\" as \"" + name + "\"", *exception);
	return nullptr;
}

void Win32FileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	String name = GetFullName(fileName);

	BEGIN_TRY();

	Win32Handle hFile = CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
	if(!hFile.IsValid())
		THROW_SECONDARY("Can't create file", Exception::SystemError());
	DWORD written;
	size_t size = file->GetSize();
	if((DWORD)size != size)
		THROW("So big files is not supported");
	if(!WriteFile(hFile, file->GetData(), (DWORD)size, &written, NULL) || written != size)
		THROW_SECONDARY("Can't write file", Exception::SystemError());

	END_TRY("Can't save file \"" + fileName + "\" as \"" + name + "\"");
}

ptr<InputStream> Win32FileSystem::LoadStream(const String& fileName)
{
	ptr<Storage> storage = LoadStorage(fileName);

	return storage->GetInputStream(0, storage->GetBigSize());
}

ptr<OutputStream> Win32FileSystem::SaveStream(const String& fileName)
{
	String name = GetFullName(fileName);

	BEGIN_TRY();

	Win32Handle file = CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
	if(!file.IsValid())
		THROW_SECONDARY("Can't open file", Exception::SystemError());
	return NEW(Win32OutputStream(std::move(file)));

	END_TRY("Can't save file \"" + fileName + "\" as \"" + name + "\" as stream");
}

time_t Win32FileSystem::GetFileMTime(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		Win32Handle hFile = CreateFile(Strings::UTF82Unicode(name).c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, NULL);
		if(!hFile.IsValid())
			THROW_SECONDARY("Can't open file", Exception::SystemError());

		FILETIME t;
		if(!GetFileTime(hFile, NULL, NULL, &t))
			THROW_SECONDARY("Can't get file's mtime", Exception::SystemError());

		ULARGE_INTEGER ull;
		ull.LowPart = t.dwLowDateTime;
		ull.HighPart = t.dwHighDateTime;

		return (time_t)(ull.QuadPart / 10000000ULL - 11644473600ULL);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't get mtime of \"") + fileName + "\"", exception);
	}
}

void Win32FileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	WIN32_FIND_DATA find;
	HANDLE hFind = FindFirstFile(Strings::UTF82Unicode(GetFullName(directoryName) + "*").c_str(), &find);
	if(hFind == INVALID_HANDLE_VALUE) return;
	do
	{
		String fileTitle(Strings::Unicode2UTF8(find.cFileName));
		if(!fileTitle.length() || fileTitle[0] == '.' || (find.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			continue;
		if(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			fileTitle += '/';
		entries.push_back(fileTitle);
	}
	while(FindNextFile(hFind, &find));
	FindClose(hFind);
}

void Win32FileSystem::MakeDirectory(const String& directoryName)
{
	if(!CreateDirectory(Strings::UTF82Unicode(GetFullName(directoryName)).c_str(), NULL))
		THROW_SECONDARY("Can't make directory " + directoryName, Exception::SystemError());
}

void Win32FileSystem::DeleteEntry(const String& entryName)
{
	std::wstring fullEntryName = Strings::UTF82Unicode(GetFullName(entryName));
	DWORD attrs = GetFileAttributes(fullEntryName.c_str());
	if(attrs == INVALID_FILE_ATTRIBUTES || !((attrs & FILE_ATTRIBUTE_DIRECTORY) ? RemoveDirectory : DeleteFile)(fullEntryName.c_str()))
		THROW_SECONDARY("Can't delete entry " + entryName, Exception::SystemError());
}

FileSystem::EntryType Win32FileSystem::GetEntryType(const String& entryName) const
{
	DWORD attrs = GetFileAttributes(Strings::UTF82Unicode(GetFullName(entryName)).c_str());
	if(attrs == INVALID_FILE_ATTRIBUTES)
	{
		switch(GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			return entryTypeMissing;
		}
		THROW_SECONDARY("Can't get entry type of " + entryName, Exception::SystemError());
	}

	if(attrs & FILE_ATTRIBUTE_DIRECTORY) return entryTypeDirectory;
	return entryTypeFile;
}

ptr<File> Win32FileSystem::LoadFile(const String& fileName)
{
	ptr<Exception> exception;
	ptr<File> file = TryLoadPartOfFile(fileName, 0, 0, &exception);
	if(file) return file;
	THROW_SECONDARY("Can't load file " + fileName, exception);
}

ptr<File> Win32FileSystem::TryLoadFile(const String& fileName)
{
	return TryLoadPartOfFile(fileName, 0, 0, nullptr);
}

ptr<Storage> Win32FileSystem::LoadStorage(const String& fileName)
{
	BEGIN_TRY();

	String name = GetFullName(fileName);
	// open file
	Win32Handle hFile = CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);
	if(!hFile.IsValid())
		THROW_SECONDARY("Can't open file", Exception::SystemError());

	LARGE_INTEGER size;
	if(!GetFileSizeEx(hFile, &size))
		THROW("Error getting file size");

	return NEW(Win32Storage(std::move(hFile), (bigsize_t)size.QuadPart));

	END_TRY("Can't load storage " + fileName);
}

ptr<Win32FileSystem> Win32FileSystem::GetNativeFileSystem()
{
	//этот метод сделан просто для лучшей понятности
	return NEW(Win32FileSystem());
}

void Win32FileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	//только если файловая система не абсолютная
	if(folderName.length())
		GetAllDirectoryEntries("/", fileNames);
}

END_INANITY_PLATFORM
