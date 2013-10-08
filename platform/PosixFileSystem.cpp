#include "PosixFileSystem.hpp"
#include "../File.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"
#include "../EmptyFile.hpp"
#include "../PartFile.hpp"
#include "../Exception.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <climits>

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
			if(readSize > SSIZE_MAX)
				readSize = SSIZE_MAX;
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
	//если имя абсолютное
	if(userFolderName.length() >= 1 && userFolderName[0] == '/')
		folderName = userFolderName;
	//иначе относительное
	else
	{
		//получить полное имя текущего каталога
		char* currentDirectory = getcwd(0, 0);
		if(!currentDirectory)
			THROW("Can't get current directory");
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

PosixFileSystem::PosixFileSystem()
{
	//создать абсолютную файловую систему, то есть ничего не делать;
	//folderName - пустая строка
}

String PosixFileSystem::GetFullName(String fileName) const
{
	if(folderName.length() && fileName.length() && fileName[0] == '/')
		fileName = fileName.substr(1);
	return folderName.length() ? (folderName + "/" + fileName) : fileName;
}

size_t PosixFileSystem::GetFileSize(const String& fileName)
{
	struct stat st;
	if(stat(fileName.c_str(), &st) != 0)
		THROW("Can't determine file size");

	return st.st_size;
}

ptr<File> PosixFileSystem::LoadPartOfFile(const String& fileName, long long mappingStart, size_t mappingSize)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(name.c_str(), O_RDONLY, 0);
		if(fd < 0)
			THROW_SECONDARY("Can't open file", Exception::SystemError());
		size_t size;
		if(mappingSize)
			size = mappingSize;
		else
		{
			struct stat st;
			if(fstat(fd, &st) < 0)
				THROW_SECONDARY("Can't get file size", Exception::SystemError());
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
			THROW_SECONDARY("Can't map file", Exception::SystemError());

		//если сдвиг был
		if(realMappingStart < (unsigned long long)mappingStart)
			//вернуть указатель на частичный файл, с учетом сдвига
			return NEW(PartFile(NEW(PosixFile(data, realMappingSize)), (char*)data + (size_t)(mappingStart - realMappingStart), size));
		//иначе сдвига не было, просто вернуть файл
		return NEW(PosixFile(data, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't load file \"" + fileName + "\" as \"" + name + "\"", exception);
	}
}

void PosixFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	String name = GetFullName(fileName);
	try
	{
		int fd = open(fileName.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
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
		int fd = open(fileName.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if(fd < 0)
			THROW_SECONDARY("Can't open file", Exception::SystemError());
		return NEW(PosixOutputStream(fd));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY(String("Can't save file \"") + fileName + "\" as \"" + name + "\" as stream", exception);
	}
}

void PosixFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	String fullDirectoryName = GetFullName(directoryName);
	DIR* dir = opendir(fullDirectoryName.c_str());

	while(struct dirent* ent = readdir(dir))
	{
		if(errno)
		{
			ptr<Exception> exception = Exception::SystemError();
			closedir(dir);
			THROW_SECONDARY("Can't read dir", exception);
		}

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
		if((st.st_mode & S_IFMT) == S_IFDIR)
			// добавить слеш в конец
			fileTitle += '/';
		// добавить файл/каталог в результат
		entries.push_back(directoryName + fileTitle);
	}

	closedir(dir);
}

ptr<File> PosixFileSystem::LoadFile(const String& fileName)
{
	return LoadPartOfFile(fileName, 0, 0);
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

END_INANITY_PLATFORM
