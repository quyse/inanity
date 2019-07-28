#include "PosixFileSystem.hpp"
#include "../File.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"
#include "../EmptyFile.hpp"
#include "../PartFile.hpp"
#include "../Exception.hpp"
#include <limits>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#if defined(___INANITY_PLATFORM_SWITCH)
#include <nn/os.h>
#endif

BEGIN_INANITY_PLATFORM

class PosixFile : public File
{
private:
	void* data;
	size_t size;

public:
	PosixFile(void* data, size_t size)
	: data(data), size(size) {}

	~PosixFile()
	{
		munmap(data, size);
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

class PosixInputStream : public InputStream
{
private:
	int fd;

public:
	PosixInputStream(int fd) : fd(fd) {}

	~PosixInputStream()
	{
		close(fd);
	}

	size_t Read(void* data, size_t size)
	{
		char* dataPtr = (char*)data;
		size_t resultSize = 0;
		while(size > 0)
		{
			ssize_t readSize = size;
			if(readSize > std::numeric_limits<ssize_t>::max())
				readSize = std::numeric_limits<ssize_t>::max();
			readSize = read(fd, dataPtr, readSize);
			if(readSize < 0)
				THROW("Disk read error");
			if(readSize == 0)
				break;
			resultSize += readSize;
			size -= readSize;
			dataPtr += readSize;
		}
		return resultSize;
	}

	size_t GetSize() const
	{
		struct stat st;
		if(fstat(fd, &st) != 0)
			THROW("Error getting size");
		return st.st_size;
	}
};

class PosixOutputStream : public OutputStream
{
private:
	int fd;

public:
	PosixOutputStream(int fd) : fd(fd) {}

	~PosixOutputStream()
	{
		close(fd);
	}

	void Write(const void* data, size_t size)
	{
		const char* dataPtr = (const char*)data;
		while(size > 0)
		{
			ssize_t written = write(fd, dataPtr, size);
			if(written < 0)
				THROW("Disk write error");
			size -= written;
			dataPtr += written;
		}
	}
};

PosixFileSystem::PosixFileSystem(const String& userFolderName)
{
	// if the name is absolute
	if(userFolderName.length() && userFolderName[0] == '/')
		folderName = userFolderName;
	// else it's relative
	else
	{
		// get full path of current directory
		char currentDirectory[1024];
		if(!getcwd(currentDirectory, sizeof(currentDirectory)))
			THROW_SECONDARY("Can't get current directory", Exception::SystemError());
		folderName = currentDirectory;

		// add specified user folder name
		if(userFolderName.length())
		{
			// add '/' to the end
			if(!folderName.length() || folderName[folderName.length() - 1] != '/')
				folderName += '/';
			folderName += userFolderName;
		}
	}

	// add '/' to the end
	if(folderName.empty() || folderName[folderName.length() - 1] != '/')
		folderName += '/';
}

PosixFileSystem::PosixFileSystem()
{
	//создать абсолютную файловую систему, то есть ничего не делать;
	//folderName - пустая строка
}

String PosixFileSystem::GetFullName(const String& fileName) const
{
	if(folderName.length())
	{
		if(fileName.empty() || fileName[0] != '/')
			THROW("File name should begin with slash");
		return folderName + fileName.substr(1);
	}
	else
		return fileName;
}

size_t PosixFileSystem::GetFileSize(const String& fileName)
{
	struct stat st;
	if(stat(fileName.c_str(), &st) != 0)
		THROW("Can't determine file size");

	return st.st_size;
}

ptr<File> PosixFileSystem::TryLoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize, ptr<Exception>* exception)
{
	String name = GetFullName(fileName);

	do
	{
		int fd = open(name.c_str(), O_RDONLY, 0);
		if(fd < 0)
		{
			if(exception)
				*exception = NEW_SECONDARY_EXCEPTION("Can't open file", Exception::SystemError());
			break;
		}
		size_t size;
		if(mappingSize)
			size = mappingSize;
		else
		{
			struct stat st;
			if(fstat(fd, &st) < 0)
			{
				close(fd);
				if(exception)
					*exception = NEW_SECONDARY_EXCEPTION("Can't get file size", Exception::SystemError());
				break;
			}
			size = st.st_size;
		}
		if(!size)
		{
			close(fd);
			return NEW(EmptyFile());
		}

		//получить размер страницы
		static size_t pageSize = 0;
		if(!pageSize)
#if defined(___INANITY_PLATFORM_SWITCH)
			pageSize = nn::os::MemoryBlockUnitSize;
#else
			pageSize = getpagesize();
#endif

		//округлить начало проекции вниз на размер страницы
		size_t realMappingStart = mappingStart & ~(pageSize - 1);
		//вычислить реальный размер
		size_t realMappingSize = size + (size_t)(mappingStart - realMappingStart);
		//спроецировать файл с учетом этого сдвига
		void* data = mmap(0, realMappingSize, PROT_READ, MAP_PRIVATE, fd, realMappingStart);
		close(fd);
		if(data == (caddr_t)-1)
		{
			if(exception)
				*exception = NEW_SECONDARY_EXCEPTION("Can't map file", Exception::SystemError());
			break;
		}

		//если сдвиг был
		if(realMappingStart < (unsigned long long)mappingStart)
			//вернуть указатель на частичный файл, с учетом сдвига
			return NEW(PartFile(NEW(PosixFile(data, realMappingSize)), (char*)data + (size_t)(mappingStart - realMappingStart), size));
		//иначе сдвига не было, просто вернуть файл
		return NEW(PosixFile(data, size));
	}
	while(false);

	if(exception)
		*exception = NEW_SECONDARY_EXCEPTION("Can't load file \"" + fileName + "\" as \"" + name + "\"", *exception);
	return nullptr;
}

void PosixFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if(fd < 0)
			THROW_SECONDARY("Can't open file", Exception::SystemError());

		const char* data = (const char*)file->GetData();
		size_t size = file->GetSize();

		while(size > 0)
		{
			ssize_t written = write(fd, data, size);
			if(written < 0)
			{
				ptr<Exception> exception = Exception::SystemError();
				close(fd);
				THROW_SECONDARY("Can't write file", exception);
			}
			size -= written;
			data += written;
		}
		close(fd);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't save file \"") + fileName + "\" as \"" + name + "\"", exception);
	}
}

ptr<InputStream> PosixFileSystem::LoadStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(name.c_str(), O_RDONLY, 0);
		if(fd < 0)
			THROW_SECONDARY("Can't open file", Exception::SystemError());
		return NEW(PosixInputStream(fd));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't load file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

ptr<OutputStream> PosixFileSystem::SaveStream(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if(fd < 0)
			THROW_SECONDARY("Can't open file", Exception::SystemError());
		return NEW(PosixOutputStream(fd));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't save file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

time_t PosixFileSystem::GetFileMTime(const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		struct stat s;
		if(stat(name.c_str(), &s) != 0)
			THROW_SECONDARY("Can't get file's mtime", Exception::SystemError());
		return s.st_mtime;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't get mtime of \"") + fileName + "\"", exception);
	}
}

void PosixFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	String fullDirectoryName = GetFullName(directoryName);
	DIR* dir = opendir(fullDirectoryName.c_str());
	if(!dir)
		THROW_SECONDARY("Can't open dir " + directoryName, Exception::SystemError());

	while(struct dirent* ent = readdir(dir))
	{
		String fileTitle = ent->d_name;
		String fullFileName = fullDirectoryName + fileTitle;
		struct stat st;
		if(stat(fullFileName.c_str(), &st) < 0)
		{
			ptr<Exception> exception = Exception::SystemError();
			closedir(dir);
			THROW_SECONDARY("Can't stat file " + fullFileName, exception);
		}

		// если файл скрыт (или ненужен: . или ..)
		if(!fileTitle.length() || fileTitle[0] == '.')
			// пропустить его
			continue;

		// если это каталог
		if(S_ISDIR(st.st_mode))
			// добавить слеш в конец
			fileTitle += '/';
		// добавить файл/каталог в результат
		entries.push_back(fileTitle);
	}

	closedir(dir);
}

void PosixFileSystem::MakeDirectory(const String& directoryName)
{
	String fullDirectoryName = GetFullName(directoryName);
	if(mkdir(fullDirectoryName.c_str(), 0755))
		THROW_SECONDARY("Can't make dir " + directoryName, Exception::SystemError());
}

void PosixFileSystem::DeleteEntry(const String& entryName)
{
	String fullEntryName = GetFullName(entryName);
	if(remove(fullEntryName.c_str()))
		THROW_SECONDARY("Can't delete entry " + entryName, Exception::SystemError());
}

FileSystem::EntryType PosixFileSystem::GetEntryType(const String& entryName) const
{
	String fullEntryName = GetFullName(entryName);
	struct stat st;
	if(!stat(fullEntryName.c_str(), &st))
	{
		if(S_ISREG(st.st_mode)) return entryTypeFile;
		if(S_ISDIR(st.st_mode)) return entryTypeDirectory;
		return entryTypeOther;
	}
	if(errno == ENOENT) return entryTypeMissing;
	THROW_SECONDARY("Can't get entry type of " + entryName, Exception::SystemError());
}

ptr<File> PosixFileSystem::LoadFile(const String& fileName)
{
	ptr<Exception> exception;
	ptr<File> file = TryLoadPartOfFile(fileName, 0, 0, &exception);
	if(file) return file;
	throw exception;
}

ptr<File> PosixFileSystem::TryLoadFile(const String& fileName)
{
	return TryLoadPartOfFile(fileName, 0, 0, nullptr);
}

ptr<PosixFileSystem> PosixFileSystem::GetNativeFileSystem()
{
	//этот метод сделан просто для лучшей понятности
	return NEW(PosixFileSystem());
}

void PosixFileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	//только если файловая система не абсолютная
	if(folderName.length())
		GetAllDirectoryEntries("/", fileNames);
}

ptr<FileSystem> PosixFileSystem::GetSubFileSystem(const String& folderName)
{
	return NEW(PosixFileSystem(GetFullName(folderName)));
}

END_INANITY_PLATFORM
