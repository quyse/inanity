#ifndef ___INANITY_RESOURCE_MANAGER_IPP___
#define ___INANITY_RESOURCE_MANAGER_IPP___

#include "ResourceManager.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"

BEGIN_INANITY

template <typename Resource>
ResourceManager<Resource>::ResourceManager(ptr<FileSystem> fileSystem)
: fileSystem(fileSystem) {}

template <typename Resource>
ptr<Resource> ResourceManager<Resource>::Get(const String& resourceName)
{
	try
	{
		// попробовать найти ресурс в кэше
		typename std::unordered_map<String, ptr<Resource> >::const_iterator i = resources.find(resourceName);
		if(i != resources.end())
			return i->second;

		// не найден, загрузить
		ptr<Resource> resource = Load(resourceName);
		// добавить в кэш
		resources[resourceName] = resource;

		return resource;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get resource " + resourceName, exception);
	}
}

END_INANITY

#endif
