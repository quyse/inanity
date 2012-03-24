#include "ResourceManager.hpp"

String ResourceManager::GetFileNamePrefix(const String& fileName)
{
	size_t slashPos = fileName.find_last_of('/');
	if(slashPos == fileName.npos)
		return String();
	return fileName.substr(0, slashPos + 1);
}

ResourceManager::ResourceManager(ptr<FileSystem> fileSystem) : fileSystem(fileSystem)
{
}
