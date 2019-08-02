#include "FileSystem.hpp"
#include "File.hpp"
#include "FileInputStream.hpp"
#include "SubFileSystem.hpp"
#include "OutputStream.hpp"
#include "Exception.hpp"

BEGIN_INANITY

ptr<File> FileSystem::LoadFile(const String& fileName)
{
	ptr<File> file = TryLoadFile(fileName);
	if(file)
		return file;
	THROW("Can't load file " + fileName);
}

ptr<File> FileSystem::TryLoadFile(const String& fileName)
{
	try
	{
		return LoadFile(fileName);
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		return 0;
	}
}

ptr<Storage> FileSystem::LoadStorage(const String& fileName)
{
	return LoadFile(fileName);
}

ptr<InputStream> FileSystem::LoadStream(const String& fileName)
{
	try
	{
		return NEW(FileInputStream(LoadFile(fileName)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't load file " + fileName + " as stream", exception);
	}
}

void FileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	THROW("Saving files in this filesystem is not supported");
}

ptr<OutputStream> FileSystem::SaveStream(const String& fileName)
{
	THROW("Saving files as stream in this filesystem is not supported");
}

time_t FileSystem::GetFileMTime(const String& fileName)
{
	THROW("Getting mtime in this filesystem is not supported");
}

void FileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	THROW("Getting file names in this filesystem is not supported");
}

void FileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	THROW("Getting directory entries in this filesystem is not supported");
}

void FileSystem::GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	// получить (нерекурсивно) файлы и каталоги в заданном каталоге
	size_t size1 = entries.size();
	GetDirectoryEntries(directoryName, entries);
	size_t size2 = entries.size();

	// перебрать полученные файлы и каталоги
	for(size_t i = size1; i < size2; ++i)
	{
		// получить файл
		String& entry = entries[i];
		// добавить к имени имя текущего каталога, чтобы сделать абсолютные имена
		entry = directoryName + entry;
		// если это каталог
		if(entry.length() && entry[entry.length() - 1] == '/')
			// рекурсивно получить файлы и каталоги в нём
			// копия имени делается, так как ссылка может стать недействительной
			GetAllDirectoryEntries(String(entry), entries);
	}
}

void FileSystem::MakeDirectory(const String& directoryName)
{
	THROW("Making directory in this filesystem is not supported");
}

void FileSystem::DeleteEntry(const String& entryName)
{
	THROW("Removing entry in this filesystem is not supported");
}

FileSystem::EntryType FileSystem::GetEntryType(const String& entryName) const
{
	THROW("Getting entry type in this filesystem is not supported");
}

ptr<FileSystem> FileSystem::GetSubFileSystem(const String& folderName)
{
	return NEW(SubFileSystem(this, folderName));
}

END_INANITY
