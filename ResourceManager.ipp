#ifndef ___INANITY_RESOURCE_MANAGER_IPP___
#define ___INANITY_RESOURCE_MANAGER_IPP___

#include "ResourceManager.hpp"
#include "ResourceLoader.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"

/* Файл содержит реализацию шаблонных методов класса
ResourceManager, определённого в ResourceManager_decl.hpp.
*/

BEGIN_INANITY

template <typename T>
void ResourceManager::AddResource(const String& fileName, ptr<T> resource)
{
	// проверить, что ресурса нет
	if(resources.find(fileName) != resources.end())
		THROW_PRIMARY_EXCEPTION("Resource with name " + fileName + " already exists");
	// добавить ресурс
	resources[fileName] = resource;
}

template <typename T>
ptr<T> ResourceManager::GetResource(const String& fileName) const
{
	try
	{
		// попробовать найти ресурс в уже загруженных
		std::unordered_map<String, ptr<Object> >::const_iterator it = resources.find(fileName);
		// если не найден
		if(it == resources.end())
			THROW_PRIMARY_EXCEPTION("Resource not loaded");
		// преобразовать к типу
		T* resource = dynamic_cast<T*>(&*it->second);
		// проверить, что есть
		if(!resource)
			THROW_PRIMARY_EXCEPTION("Invalid resource type");
		// вернуть ресурс
		return resource;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get resource " + fileName, exception);
	}
}

template <typename T>
ptr<T> ResourceManager::LoadResource(const String& fileName)
{
	try
	{
		// попробовать найти ресурс в уже загруженных
		std::unordered_map<String, ptr<Object> >::const_iterator it = resources.find(fileName);
		// если найден
		if(it != resources.end())
		{
			// преобразовать к типу
			T* resource = dynamic_cast<T*>(&*it->second);
			// проверить, что есть
			if(!resource)
				THROW_PRIMARY_EXCEPTION("Invalid resource type");
			// вернуть ресурс
			return resource;
		}

		// иначе ресурс ещё не загружен
		// загрузить новый ресурс
		ptr<T> resource = T::Deserialize(fileSystem->LoadFile(fileName), &ResourceLoader(this, GetFileNamePrefix(fileName)));
		// добавить его в карту ресурсов
		resources[fileName] = resource;
		// вернуть ресурс
		return resource;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load resource \"" + fileName + "\"", exception);
	}
}

template <typename T>
void ResourceManager::AddStaticResource(ptr<T> resource)
{
	AddResource<T>(T::staticResourceName, resource);
}

template <typename T>
ptr<T> ResourceManager::GetStaticResource() const
{
	return GetResource<T>(T::staticResourceName);
}

template <typename T>
void ResourceManager::CreateStaticResource()
{
	AddResource<T>(T::staticResourceName, T::CreateStaticResource(&ResourceLoader(this, GetFileNamePrefix(T::staticResourceName))));
}

template <typename T>
ptr<T> ResourceManager::LoadStaticResource()
{
	// проверить, что ресурс есть; если нет - загрузить его
	if(resources.find(T::staticResourceName) == resources.end())
		CreateStaticResource<T>();
	return GetStaticResource<T>();
}

END_INANITY

#endif
