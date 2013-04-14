#include "FolderFileSystem.hpp"
#include "FolderFile.hpp"
#include "Handle.hpp"
#include "PartFile.hpp"
#include "EmptyFile.hpp"
#include "DiskInputStream.hpp"
#include "DiskOutputStream.hpp"
#include "Exception.hpp"
#include "scripting_impl.hpp"

BEGIN_INANITY

SCRIPTABLE_MAP_BEGIN(FolderFileSystem, Inanity.FolderFileSystem);
	SCRIPTABLE_PARENT(FileSystem);
	SCRIPTABLE_CONSTRUCTOR(FolderFileSystem, const String&);
	SCRIPTABLE_METHOD(FolderFileSystem, GetNativeFileSystem);
	SCRIPTABLE_METHOD(FolderFileSystem, GetFileSize);
	SCRIPTABLE_METHOD(FolderFileSystem, LoadPartOfFile);
SCRIPTABLE_MAP_END();

END_INANITY

#ifdef ___INANITY_WINDOWS

#include "windows.hpp"
#include "Strings.hpp"

BEGIN_INANITY

FolderFileSystem::FolderFileSystem(const String& userFolderName)
{
	//если имя абсолютное
	if(userFolderName.length() >= 2 && userFolderName[1] == ':')
		folderName = userFolderName;
	//иначе относительное
	else
	{
		//получить полное имя текущего каталога
		size_t length = GetCurrentDirectory(0, NULL);
		wchar_t* buffer = new wchar_t[length];
		if(!GetCurrentDirectory(length, buffer))
		{
			delete [] buffer;
			THROW_PRIMARY_EXCEPTION("Can't get current directory");
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

FolderFileSystem::FolderFileSystem()
{
	//создать абсолютную файловую систему, то есть ничего не делать;
	//folderName - пустая строка
}

String FolderFileSystem::GetFullName(String fileName) const
{
	if(fileName.length() && fileName.front() == '/')
		fileName = fileName.substr(1);
	String result = folderName.length() ? (folderName + "\\" + fileName) : fileName;
	size_t length = result.length();
	for(size_t i = 0; i < length; ++i)
		if(result[i] == '/')
			result[i] = '\\';
	return result;
}

size_t FolderFileSystem::GetFileSize(const String& fileName)
{
	Handle hFile = CreateFile(Strings::UTF82Unicode(GetFullName(fileName)).c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(!hFile.IsValid())
		THROW_PRIMARY_EXCEPTION("Can't open file to determine file size");

	LARGE_INTEGER size;
	if(!::GetFileSizeEx(hFile, &size))
		THROW_PRIMARY_EXCEPTION("Can't get file size");

	size_t resultSize = (size_t)size.QuadPart;
	if(resultSize != size.QuadPart)
		THROW_PRIMARY_EXCEPTION("File is too big");
	
	return resultSize;
}

ptr<File> FolderFileSystem::LoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize)
{
	String name = GetFullName(fileName);
	try
	{
		Handle hFile = CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, NULL);
		if(!hFile.IsValid())
			THROW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
		size_t size;
		if(mappingSize)
			size = mappingSize;
		else
		{
			LARGE_INTEGER li;
			::GetFileSizeEx(hFile, &li);
			size = (size_t)li.QuadPart;
			if(size != li.QuadPart)
				THROW_PRIMARY_EXCEPTION("File too long to map");
		}
		if(!size)
			return NEW(EmptyFile());
		Handle hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		if(!hMapping)
			THROW_SECONDARY_EXCEPTION("Can't create file mapping", Exception::SystemError());

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
			THROW_SECONDARY_EXCEPTION("Can't map view of file", Exception::SystemError());

		//если сдвиг был
		if(realMappingStart < mappingStart)
			//вернуть указатель на частичный файл, с учетом сдвига
			return NEW(PartFile(NEW(FolderFile(data, realMappingSize)), (char*)data + (size_t)(mappingStart - realMappingStart), size));
		//иначе сдвига не было, просто вернуть файл
		return NEW(FolderFile(data, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load file \"" + fileName + "\" as \"" + name + "\"", exception);
	}
}

void FolderFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		Handle hFile = CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
		if(!hFile.IsValid())
			THROW_SECONDARY_EXCEPTION("Can't create file", Exception::SystemError());
		DWORD written;
		size_t size = file->GetSize();
		if((DWORD)size != size)
			THROW_PRIMARY_EXCEPTION("So big files is not supported");
		if(!WriteFile(hFile, file->GetData(), (DWORD)size, &written, NULL) || written != size)
			THROW_SECONDARY_EXCEPTION("Can't write file", Exception::SystemError());
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(String("Can't save file \"") + fileName + "\" as \"" + name + "\"", exception);
	}
}

ptr<InputStream> FolderFileSystem::LoadStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		ptr<Handle> file = NEW(Handle(CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)));
		if(!file->IsValid())
			THROW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
		return NEW(DiskInputStream(file));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(String("Can't load file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

ptr<OutputStream> FolderFileSystem::SaveStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		ptr<Handle> file = NEW(Handle(CreateFile(Strings::UTF82Unicode(name).c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL)));
		if(!file->IsValid())
			THROW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
		return NEW(DiskOutputStream(file));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(String("Can't save file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

void FolderFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
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
		entries.push_back(directoryName + fileTitle + "/");
	}
	while(FindNextFile(hFind, &find));
	FindClose(hFind);
}

END_INANITY

#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

BEGIN_INANITY

FolderFileSystem::FolderFileSystem(const String& userFolderName)
{
	//если имя абсолютное
	if(userFolderName.length() >= 1 && userFolderName[0] == '/')
		folderName = userFolderName;
	//иначе относительное
	else
	{
		//получить полное имя текущего каталога
		char* currentDirectory = getcwd(0, 0);
		if(!currentDirectory)
			THROW_PRIMARY_EXCEPTION("Can't get current directory");
		folderName = currentDirectory;
		free(currentDirectory);

		//прибавить к нему заданное имя каталога, и получить таким образом полный каталог
		if(userFolderName.length())
		{
			folderName += '/';
			folderName += userFolderName;
		}
	}
}

FolderFileSystem::FolderFileSystem()
{
	//создать абсолютную файловую систему, то есть ничего не делать;
	//folderName - пустая строка
}

String FolderFileSystem::GetFullName(String fileName) const
{
	if(fileName.length() && fileName[0] == '/')
		fileName = fileName.substr(1);
	return folderName.length() ? (folderName + "/" + fileName) : fileName;
}

size_t FolderFileSystem::GetFileSize(const String& fileName)
{
	struct stat st;
	if(stat(fileName.c_str(), &st) != 0)
		THROW_PRIMARY_EXCEPTION("Can't determine file size");

	return st.st_size;
}

ptr<File> FolderFileSystem::LoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(name.c_str(), O_RDONLY, 0);
		if(fd < 0)
			THROW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
		size_t size;
		if(mappingSize)
			size = mappingSize;
		else
		{
			struct stat st;
			if(fstat(fd, &st) < 0)
				THROW_SECONDARY_EXCEPTION("Can't get file size", Exception::SystemError());
			size = st.st_size;
		}
		if(!size)
			return NEW(EmptyFile());

		//получить размер страницы
		static size_t pageSize = 0;
		if(!pageSize)
			pageSize = getpagesize();

		//округлить начало проекции вниз на размер страницы
		size_t realMappingStart = mappingStart & ~(pageSize - 1);
		//вычислить реальный размер
		size_t realMappingSize = size + (size_t)(mappingStart - realMappingStart);
		//спроецировать файл с учетом этого сдвига
		void* data = mmap(0, realMappingSize, PROT_READ, MAP_PRIVATE, fd, realMappingStart);
		if(data == (caddr_t)-1)
			THROW_SECONDARY_EXCEPTION("Can't map file", Exception::SystemError());

		//если сдвиг был
		if(realMappingStart < (unsigned long long)mappingStart)
			//вернуть указатель на частичный файл, с учетом сдвига
			return NEW(PartFile(NEW(FolderFile(data, realMappingSize)), (char*)data + (size_t)(mappingStart - realMappingStart), size));
		//иначе сдвига не было, просто вернуть файл
		return NEW(FolderFile(data, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load file \"" + fileName + "\" as \"" + name + "\"", exception);
	}
}

void FolderFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(fileName.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if(fd < 0)
			THROW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());

		const char* data = (const char*)file->GetData();
		size_t size = file->GetSize();

		while(size > 0)
		{
			ssize_t written = write(fd, data, size);
			if(written < 0)
			{
				ptr<Exception> exception = Exception::SystemError();
				close(fd);
				THROW_SECONDARY_EXCEPTION("Can't write file", exception);
			}
			size -= written;
			data += written;
		}
		close(fd);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(String("Can't save file \"") + fileName + "\" as \"" + name + "\"", exception);
	}
}

ptr<InputStream> FolderFileSystem::LoadStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(name.c_str(), O_RDONLY, 0);
		if(fd < 0)
			THROW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
		return NEW(DiskInputStream(NEW(Handle(fd))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(String("Can't load file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

ptr<OutputStream> FolderFileSystem::SaveStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(fileName.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if(fd < 0)
			THROW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
		return NEW(DiskOutputStream(NEW(Handle(fd))));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(String("Can't save file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

void FolderFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	String fullDirectoryName = GetFullName(directoryName);
	DIR* dir = opendir(fullDirectoryName.c_str());

	while(struct dirent* ent = readdir(dir))
	{
		if(errno)
		{
			ptr<Exception> exception = Exception::SystemError();
			closedir(dir);
			THROW_SECONDARY_EXCEPTION("Can't read dir", exception);
		}

		String fileTitle = ent->d_name;
		String fullFileName = fullDirectoryName + fileTitle;
		struct stat st;
		if(stat(fullFileName.c_str(), &st) < 0)
		{
			ptr<Exception> exception = Exception::SystemError();
			closedir(dir);
			THROW_SECONDARY_EXCEPTION("Can't stat file " + fullFileName, exception);
		}

		// если файл скрыт (или ненужен: . или ..)
		if(!fileTitle.length() || fileTitle[0] == '.')
			// пропустить его
			continue;

		// если это каталог
		if((st.st_mode & S_IFMT) == S_IFDIR)
			// добавить слеш в конец
			fileTitle += '/';
		// добавить файл/каталог в результат
		entries.push_back(directoryName + fileTitle);
	}

	closedir(dir);
}

END_INANITY

#endif // ___INANITY_LINUX

BEGIN_INANITY

ptr<File> FolderFileSystem::LoadFile(const String& fileName)
{
	return LoadPartOfFile(fileName, 0, 0);
}

ptr<FolderFileSystem> FolderFileSystem::GetNativeFileSystem()
{
	//этот метод сделан просто для лучшей понятности
	return NEW(FolderFileSystem());
}

void FolderFileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	//только если файловая система не абсолютная
	if(folderName.length())
		GetAllDirectoryEntries("/", fileNames);
}

END_INANITY
