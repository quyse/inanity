#ifndef ___INANITY_RESOURCE_LOADER_HPP___
#define ___INANITY_RESOURCE_LOADER_HPP___

#include "String.hpp"

BEGIN_INANITY

class ResourceManager;

/// Вспомогательный класс для загрузки связанных ресурсов.
/** Просто разрешает относительные ссылки. */
class ResourceLoader
{
private:
	/// Менеджер ресурсов.
	ptr<ResourceManager> resourceManager;
	/// Префикс пути (со слешем в конце).
	const String pathPrefix;

public:
	ResourceLoader(ptr<ResourceManager> resourceManager, const String& pathPrefix);

	/// Преобразовать имя ресурса в соответствии с состоянием загрузчика.
	String TransformName(const String& fileName) const;

	/// Получить менеджер ресурсов.
	ptr<ResourceManager> GetResourceManager() const;

	/// Загрузить ресурс.
	template <typename T>
	ptr<T> LoadResource(const String& fileName)
	{
		return resourceManager->LoadResource<T>(TransformName(fileName));
	}

	/// Получить ресурс.
	template <typename T>
	ptr<T> GetResource(const String& fileName)
	{
		return resourceManager->GetResource<T>(TransformName(fileName));
	}

	/// Получить статический ресурс.
	template <typename T>
	ptr<T> GetStaticResource()
	{
		return resourceManager->GetStaticResource<T>();
	}

	/// Загрузить статический ресурс.
	template <typename T>
	ptr<T> LoadStaticResource()
	{
		return resourceManager->LoadStaticResource<T>();
	}
};

END_INANITY

#endif
