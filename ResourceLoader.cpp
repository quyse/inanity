#include "ResourceLoader.hpp"
#include "ResourceManager.hpp"

ResourceLoader::ResourceLoader(ptr<ResourceManager> resourceManager, const String& pathPrefix)
	: resourceManager(resourceManager), pathPrefix(pathPrefix)
{
}

String ResourceLoader::TransformName(const String& fileName) const
{
	// если имя файла не начинается на слеш, то это относительное имя
	if(fileName.length() > 0 && fileName[0] != '/')
		// добавить к имени префикс
		return pathPrefix + fileName;
	// вернуть абсолютное имя
	return fileName;
}

ptr<ResourceManager> ResourceLoader::GetResourceManager() const
{
	return resourceManager;
}
