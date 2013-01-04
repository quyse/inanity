#ifndef ___INANITY_RESOURCE_LOADER_IPP___
#define ___INANITY_RESOURCE_LOADER_IPP___

#include "ResourceLoader.hpp"
#include "ResourceManager.hpp"

BEGIN_INANITY

/* Файл содержит реализацию шаблонных методов класса
ResourceLoader, определённого в ResourceLoader_decl.hpp.
*/

template <typename T>
ptr<T> ResourceLoader::LoadResource(const String& fileName)
{
	return resourceManager->LoadResource<T>(TransformName(fileName));
}

template <typename T>
ptr<T> ResourceLoader::GetResource(const String& fileName)
{
	return resourceManager->GetResource<T>(TransformName(fileName));
}

template <typename T>
ptr<T> ResourceLoader::GetStaticResource()
{
	return resourceManager->GetStaticResource<T>();
}

template <typename T>
ptr<T> ResourceLoader::LoadStaticResource()
{
	return resourceManager->LoadStaticResource<T>();
}

END_INANITY

#endif
