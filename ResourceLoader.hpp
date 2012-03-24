#ifndef ___INANITY_RESOURCE_LOADER_HPP___
#define ___INANITY_RESOURCE_LOADER_HPP___

#include "Object.hpp"

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
	String pathPrefix;

public:
	Loader(ptr<ResourceManager> resourceManager, const String& pathPrefix);

	/// Загрузить ресурс.
	template <typename T>
	ptr<T> LoadResource(String fileName)
	{
		// если имя файла не начинается на слеш, то это относительное имя
		if(fileName.length() > 0 && fileName[0] != '/')
			// добавить к имени префикс
			fileName = pathPrefix + fileName;

		// загрузить ресурс
		return resourceManager->LoadResource<T>(fileName);
	}
};

END_INANITY

#endif
