#include "ResourceStore.hpp"

ResourceStore::ResourceStore(ptr<FileSystem> fileSystem) : fileSystem(fileSystem)
{
}

String ResourceStore::GetFileNameExtension(const String& fileName)
{
	size_t i = fileName.find_last_of('.');
	if(i == String::npos)
		return String();
	return fileName.substr(i + 1);
}

void ResourceStore::RegisterResourceLoader(const String& fileNameExtension, ptr<ResourceLoader> resourceLoader)
{
	resourceLoaders[fileNameExtension] = resourceLoader;
}
