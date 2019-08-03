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

class Win32InputStream : public InputStream
{
private:
	ptr<Win32Handle> handle;
	mutable bigsize_t fileSize;

	bigsize_t GetFileSize() const
	{
		if(fileSize == (bigsize_t)-1)
		{
			LARGE_INTEGER size;
			if(!GetFileSizeEx(*handle, &size))
				THROW("Error getting file size");
			fileSize = (bigsize_t)size.QuadPart;
		}
		return fileSize;
	}

public:
	Win32InputStream(ptr<Win32Handle> handle)
	: handle(handle), fileSize(-1) {}

	size_t Read(void* data, size_t size)
	{
		if((DWORD)size != size)
			THROW("So big reading size is not supported");
		DWORD read;
		ReadFile(*handle, data, (DWORD)size, &read, NULL);
		return read;
	}

	bigsize_t Skip(bigsize_t size)
	{
		LARGE_INTEGER distance, oldFilePointer, newFilePointer;

		// determine current file pointer
		distance.QuadPart = (LONGLONG)0;
		if(!SetFilePointerEx(*handle, distance, &oldFilePointer, FILE_CURRENT))
			THROW("Can't determine current file pointer");

		// get file size
		bigsize_t fileSize = GetFileSize();

		// fix distance to move (because Windows allows
		// setting file pointer beyond the end of file)
		size = std::min((bigsize_t)oldFilePointer.QuadPart + size, fileSize) - (bigsize_t)oldFilePointer.QuadPart;

		// move
		distance.QuadPart = (LONGLONG)size;
		if(!SetFilePointerEx(*handle, distance, &newFilePointer, FILE_CURRENT))
			THROW("Can't move file pointer");

		return size;
	}
};

class Win32OutputStream : public OutputStream
{
private:
	ptr<Win32Handle> handle;

public:
	Win32OutputStream(ptr<Win32Handle> handle)
	: handle(handle) {}

	void Write(const void* data, size_t size)
	{
		if((DWORD)size != size)
			THROW("So big write size is not supported");
		DWORD written;
		if(!WriteFile(*handle, data, (DWORD)size, &written, NULL) || written != size)
			THROW("Disk write error");
	}
};

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

	//убедиться, что имя каталога поддерживает длинные имена
	if(this->folderName.compare(0, 4, "\\\\?\\") != 0)
		this->folderName = "\\\\?\\" + this->folderName;
}

Win32FileSystem::Win32FileSystem()
{
	//создать абсолютную файловую систему, то есть ничего не делать;
	//folderName - пустая строка
}

String Win32FileSystem::GetFullName(String fileName) const
{
	if(folderName.length() && fileName.length() && fileName.front() == '/')
		fileName = fileName.substr(1);
	String result = folderName.length() ? (folderName + "\\" + fileName) : fileName;
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
	try
	{
		Win32Handle hFile = CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
		if(!hFile.IsValid())
			THROW_SECONDARY("Can't create file", Exception::SystemError());
		DWORD written;
		size_t size = file->GetSize();
		if((DWORD)size != size)
			THROW("So big files is not supported");
		if(!WriteFile(hFile, file->GetData(), (DWORD)size, &written, NULL) || written != size)
			THROW_SECONDARY("Can't write file", Exception::SystemError());
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't save file \"") + fileName + "\" as \"" + name + "\"", exception);
	}
}

ptr<InputStream> Win32FileSystem::LoadStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		ptr<Win32Handle> file = NEW(Win32Handle(CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)));
		if(!file->IsValid())
			THROW_SECONDARY("Can't open file", Exception::SystemError());
		return NEW(Win32InputStream(file));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't load file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

ptr<OutputStream> Win32FileSystem::SaveStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		ptr<Win32Handle> file = NEW(Win32Handle(CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL)));
		if(!file->IsValid())
			THROW_SECONDARY("Can't open file", Exception::SystemError());
		return NEW(Win32OutputStream(file));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't save file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
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
