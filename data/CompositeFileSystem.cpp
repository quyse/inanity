#include "CompositeFileSystem.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_DATA

String CompositeFileSystem::TryFileSystemForPath(size_t i, const String& path)
{
	//перебрать файловые системы
	const String& prefix = fileSystems[i].first;
	//если путь начинается с пути примонтированной файловой системы
	if(path.compare(0, prefix.length(), prefix, 0, prefix.length()) == 0)
	{
		// исправить путь (отрезать префикс)
		return path.substr(prefix.length() - 1);
	}

	return String();
}

void CompositeFileSystem::Mount(ptr<FileSystem> fileSystem, String path)
{
	//если в конце нет слеша, добавить его
	if(!path.length() || path.back() != '/')
		path += '/';
	fileSystems.push_back(std::make_pair(path, fileSystem));
}

ptr<File> CompositeFileSystem::TryLoadFile(const String& fileName)
{
	for(size_t i = 0; i < fileSystems.size(); ++i)
	{
		String path = TryFileSystemForPath(i, fileName);
		if(!path.empty())
		{
			ptr<File> file = fileSystems[i].second->TryLoadFile(path);
			if(file) return file;
		}
	}

	return nullptr;
}

END_INANITY_DATA
