#include "FolderFileSystem.hpp"
#include "File.hpp"
#include "PartFile.hpp"
#include "EmptyFile.hpp"
#include "Handle.hpp"
#include "DiskInputStream.hpp"
#include "DiskOutputStream.hpp"
#include "Exception.hpp"
#include <windows.h>

class FolderFile : public File
{
private:
	void* data;
	size_t size;

public:
	FolderFile(void* data, size_t size) : data(data), size(size)
	{
	}

	~FolderFile()
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

FolderFileSystem::FolderFileSystem(const std::wstring& userFolderName)
{
	//если имя абсолютное
	if(userFolderName.length() >= 2 && userFolderName[1] == L':')
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
			THROW_PRIMARY_EXCEPTION(L"Can't get current directory");
		}
		folderName = buffer;
		delete [] buffer;

		//прибавить к нему заданное имя каталога, и получить таким образом полный каталог
		if(userFolderName.length())
		{
			folderName += L'\\';
			folderName += userFolderName;
		}
	}

	//убедиться, что имя каталога поддерживает длинные имена
	if(this->folderName.compare(0, 4, L"\\\\?\\") != 0)
		this->folderName = L"\\\\?\\" + this->folderName;
}

FolderFileSystem::FolderFileSystem()
{
	//создать абсолютную файловую систему, то есть ничего не делать;
	//folderName - пустая строка
}

ptr<FolderFileSystem> FolderFileSystem::GetNativeFileSystem()
{
	//этот метод сделан просто для лучшей понятности
	return NEW(FolderFileSystem());
}

std::wstring FolderFileSystem::GetFullName(std::wstring fileName) const
{
	if(fileName.length() && fileName.front() == '/')
		fileName = fileName.substr(1);
	std::wstring result = folderName.length() ? (folderName + L"\\" + fileName) : fileName;
	size_t length = result.length();
	for(size_t i = 0; i < length; ++i)
		if(result[i] == '/')
			result[i] = '\\';
	return result;
}

void FolderFileSystem::ThrowFileError()
{
	const wchar_t* message;
	switch(GetLastError())
	{
	case ERROR_FILE_NOT_FOUND:
		message = L"File not found";
		break;
	case ERROR_PATH_NOT_FOUND:
		message = L"Path not found";
		break;
	case ERROR_ACCESS_DENIED:
		message = L"Access denied";
		break;
	default:
		message = L"Unknown error when opening the file";
		break;
	}
	THROW_PRIMARY_EXCEPTION(message);
}

ptr<File> FolderFileSystem::LoadFile(const std::wstring& fileName)
{
	return LoadFile(fileName, cacheHintNone);
}

size_t FolderFileSystem::GetFileSize(const std::wstring& fileName)
{
	HANDLE file = CreateFile(GetFullName(fileName).c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(file == INVALID_HANDLE_VALUE)
		THROW_PRIMARY_EXCEPTION(L"Can't open file to determine file size");

	LARGE_INTEGER size;
	if(!::GetFileSizeEx(file, &size))
		THROW_PRIMARY_EXCEPTION(L"Can't get file size");
	CloseHandle(file);

	size_t resultSize = (size_t)size.QuadPart;
	if(resultSize != size.QuadPart)
		THROW_PRIMARY_EXCEPTION(L"File is too big");
	
	return resultSize;
}

ptr<File> FolderFileSystem::LoadFile(const std::wstring& fileName, CacheHint cacheHint)
{
	return LoadPartOfFile(fileName, 0, 0, cacheHint);
}

ptr<File> FolderFileSystem::LoadPartOfFile(const std::wstring& fileName, long long mappingStart, size_t mappingSize, CacheHint cacheHint)
{
	std::wstring name = GetFullName(fileName);
	try
	{
		DWORD flags = 0;
		switch(cacheHint)
		{
		case cacheHintSequentialScan:
			flags = FILE_FLAG_SEQUENTIAL_SCAN;
			break;
		case cacheHintRandomScan:
			flags = FILE_FLAG_RANDOM_ACCESS;
			break;
		}

		Handle hFile = CreateFile(name.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, flags, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			ThrowFileError();
		size_t size;
		if(mappingSize)
			size = mappingSize;
		else
		{
			LARGE_INTEGER li;
			::GetFileSizeEx(hFile, &li);
			size = (size_t)li.QuadPart;
			if(size != li.QuadPart)
				THROW_PRIMARY_EXCEPTION(L"File too long to map");
		}
		if(!size)
			return NEW(EmptyFile());
		Handle hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		if(!hMapping) THROW_PRIMARY_EXCEPTION(L"Can't create file mapping");

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
		if(!data) THROW_PRIMARY_EXCEPTION(L"Can't map view of file");

		//если сдвиг был
		if(realMappingStart < mappingStart)
			//вернуть указатель на частичный файл, с учетом сдвига
			return NEW(PartFile(NEW(FolderFile(data, realMappingSize)), (char*)data + (size_t)(mappingStart - realMappingStart), size));
		//иначе сдвига не было, просто вернуть файл
		return NEW(FolderFile(data, size));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(L"Can't load file \"" + fileName + L"\" as \"" + name + L"\"", exception);
	}
}

void FolderFileSystem::SaveFile(ptr<File> file, const std::wstring& fileName)
{
	std::wstring name = GetFullName(fileName);
	try
	{
		HANDLE hFile = CreateFile(name.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE) THROW_PRIMARY_EXCEPTION(L"Can't create file");
		DWORD written;
		size_t size = file->GetSize();
		if((DWORD)size != size)
			THROW_PRIMARY_EXCEPTION(L"So big files is not supported");
		if(!WriteFile(hFile, file->GetData(), (DWORD)size, &written, NULL) || written != size)
			THROW_PRIMARY_EXCEPTION(L"Can't write file");
		CloseHandle(hFile);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(std::wstring(L"Can't save file \"") + fileName + L"\" as \"" + name + L"\"", exception);
	}
}

ptr<InputStream> FolderFileSystem::LoadFileAsStream(const std::wstring& fileName)
{
	std::wstring name = GetFullName(fileName);
	try
	{
		ptr<Handle> file = NEW(Handle(CreateFile(name.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)));
		if(!file->IsValid())
			ThrowFileError();
		return NEW(DiskInputStream(file));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(std::wstring(L"Can't load file \"") + fileName + L"\" as \"" + name + L"\" as stream", exception);
	}
}

ptr<OutputStream> FolderFileSystem::SaveFileAsStream(const std::wstring& fileName)
{
	std::wstring name = GetFullName(fileName);
	try
	{
		ptr<Handle> file = NEW(Handle(CreateFile(name.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL)));
		if(!file->IsValid())
			ThrowFileError();
		return NEW(DiskOutputStream(file));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(std::wstring(L"Can't save file \"") + fileName + L"\" as \"" + name + L"\" as stream", exception);
	}
}

void FolderFileSystem::GetFileNames(std::wstring sourceDirectory, const std::wstring& targetDirectory, std::vector<std::wstring>& fileNames) const
{
	sourceDirectory += L'\\';
	WIN32_FIND_DATA find;
	HANDLE hFind = FindFirstFile((sourceDirectory + L"*").c_str(), &find);
	if(hFind == INVALID_HANDLE_VALUE) return;
	do
	{
		std::wstring fileTitle(find.cFileName);
		if(fileTitle.length() && fileTitle[0] == L'.' || (find.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
			continue;
		if(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			GetFileNames(sourceDirectory + fileTitle, targetDirectory + fileTitle + L"/", fileNames);
		else
			fileNames.push_back(targetDirectory + fileTitle);
	}
	while(FindNextFile(hFind, &find));
	FindClose(hFind);
}

void FolderFileSystem::GetFileNames(std::vector<std::wstring>& fileNames) const
{
	//только если файловая система не абсолютная
	if(folderName.length())
		GetFileNames(folderName, L"/", fileNames);
}
