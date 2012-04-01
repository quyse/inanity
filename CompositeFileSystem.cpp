#include "CompositeFileSystem.hpp"
#include "File.hpp"
#include "Exception.hpp"

ptr<FileSystem> CompositeFileSystem::GetFileSystemForPath(String& path)
{
	//перебрать файловые системы
	for(size_t i = 0; i < fileSystems.size(); ++i)
	{
		const String& prefix = fileSystems[i].first;
		//если путь начинается с пути примонтированной файловой системы
		if(path.compare(0, prefix.length(), prefix, 0, prefix.length()) == 0)
		{
			// исправить путь (отрезать префикс)
			path = path.substr(prefix.length() - 1);
			// вернуть файловую систему
			return fileSystems[i].second;
		}
	}
	// файловая система не найдена
	return nullptr;
}

void CompositeFileSystem::MountFileSystem(ptr<FileSystem> fileSystem)
{
	MountFileSystem("/", fileSystem);
}

void CompositeFileSystem::MountFileSystem(String path, ptr<FileSystem> fileSystem)
{
	//если в конце нет слеша, добавить его
	if(!path.length() || path.back() != '/')
		path += '/';
	fileSystems.push_back(std::make_pair(path, fileSystem));
}

ptr<File> CompositeFileSystem::LoadFile(const String& fileName)
{
	try
	{
		String path = fileName;
		ptr<FileSystem> fileSystem = GetFileSystemForPath(path);
		if(!fileSystem)
			THROW_PRIMARY_EXCEPTION("No mounted file system for such path");
		try
		{
			return fileSystem->LoadFile(path);
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Mounted file system opening file exception", exception);
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load file " + fileName + " from composite file system", exception);
	}
}

ptr<File> CompositeFileSystem::TryLoadFile(const String& fileName)
{
	String path = fileName;
	ptr<FileSystem> fileSystem = GetFileSystemForPath(path);
	if(!fileSystem)
		return nullptr;
	return fileSystem->TryLoadFile(path);
}

void CompositeFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	try
	{
		String path = fileName;
		ptr<FileSystem> fileSystem = GetFileSystemForPath(path);
		if(!fileSystem)
			THROW_PRIMARY_EXCEPTION("No mounted file system for such path");
		try
		{
			fileSystem->SaveFile(file, path);
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Mounted file system saving file exception", exception);
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't save file " + fileName + " into composite file system", exception);
	}
}
