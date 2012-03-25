#include "ResourceLoader.hpp"
#include "ResourceManager.hpp"

ResourceLoader::ResourceLoader(ptr<ResourceManager> resourceManager, const String& pathPrefix)
	: resourceManager(resourceManager), pathPrefix(pathPrefix)
{
}
