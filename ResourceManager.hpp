#ifndef ___INANITY_RESOURCE_MANAGER_HPP___
#define ___INANITY_RESOURCE_MANAGER_HPP___

#include "FileSystem.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include <hash_map>

BEGIN_INANITY

class ResourceLoader;

/// Класс менеджера ресурсов.
/** Загружает ресурсы, и кэширует их для исключения повторной загрузки.
Класс ресурса T должен иметь метод десериализации:
static ptr<T> Deserialize(ptr<File> file, ResourceLoader* resourceLoader);
*/
class ResourceManager : public Object
{
private:
	/// Файловая система, из которой берутся ресурсы.
	ptr<FileSystem> fileSystem;
	/// Карта уже загруженных ресурсов.
	std::hash_map<String, std::pair<ptr<Object>, void*> > resources;

	/// Получить префикс для имени файла.
	String GetFileNamePrefix(const String& fileName);

public:
	ResourceManager(ptr<FileSystem> fileSystem);

	/// Загрузить ресурс.
	template <typename T>
	ptr<T> LoadResource(const String& fileName)
	{
		try
		{
			// попробовать найти ресурс в уже загруженных
			std::hash_map<String, std::pair<ptr<Object>, void*> >::const_iterator it = resources.find(fileName);
			// если найден
			if(it != resources.end())
			{
				// проверить тип ресурса
				if(it->second.second != (void*)T::Deserialize)
					THROW_PRIMARY_EXCEPTION("Invalid resource type");
				// вернуть ресурс
				return static_cast<T*>(&*it->second.first);
			}

			// иначе ресурс ещё не загружен
			// загрузить новый ресурс
			ptr<T> resource = T::Deserialize(fileSystem->LoadFile(fileName), &ResourceLoader(this, GetFileNamePrefix(fileName)));
			// добавить его в карту ресурсов
			resources[fileName] = std::make_pair(resource, (void*)T::Deserialize);
			// вернуть ресурс
			return resource;
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't load resource \"" + fileName + "\"", exception);
		}
	}
};

END_INANITY

#endif
