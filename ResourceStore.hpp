#ifndef ___INANITY_RESOURCE_STORE_HPP___
#define ___INANITY_RESOURCE_STORE_HPP___

#include "Object.hpp"
#include "String.hpp"
#include "File.hpp"
#include "FileSystem.hpp"
#include "ResourceLoader.hpp"
#include "Exception.hpp"
#include <hash_map>

BEGIN_INANITY

class ResourceLoader;

/// Класс хранилища ресурсов.
/** Предоставляет доступ к загрузке ресурсов из указанной файловой системы.
 * Также управляет загрузчиками типов ресурсов. */
class ResourceStore : public Object
{
private:
	/// Файловая система.
	ptr<FileSystem> fileSystem;
	/// Карта загрузчиков ресурсов.
	std::hash_map<String, ptr<ResourceLoader>> resourceLoaders;

	//получить расширение файла
	static String GetFileNameExtension(const String& fileName);

public:
	ResourceStore(ptr<FileSystem> fileSystem);

	/// Зарегистрировать загрузчик ресурсов
	void RegisterResourceLoader(const String& fileNameExtension, ptr<ResourceLoader> resourceLoader);

	/// Загрузить ресурс.
	template <typename T>
	ptr<T> LoadResource(const String& fileName)
	{
		try
		{
			// получить расширение имени файла
			const String fileNameExtension = GetFileNameExtension(fileName);

			// получить загрузчик ресурсов
			std::hash_map<String, ptr<ResourceLoader> >::const_iterator it = resourceLoaders.find(fileNameExtension);
			if(it == resourceLoaders.end())
				THROW_PRIMARY_EXCEPTION("There are no loader which recognize this filename extension");
			ptr<ResourceLoader> resourceLoader = it->second;

			// проверить, что это тот загрузчик, который нужен
			if(resourceLoader->GetResourceCode() != T::resourceCode)
				THROW_PRIMARY_EXCEPTION("Invalid resource type");

			// загрузить файл
			ptr<File> file = fileSystem->LoadFile(fileName);

			// загрузить и вернуть ресурс
			return static_cast<T*>(&*resourceLoader->Load(file));
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't load resource: " + fileName, exception);
		}
	}
};

END_INANITY

#endif
