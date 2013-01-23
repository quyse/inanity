#ifndef ___INANITY_RESOURCE_MANAGER_HPP___
#define ___INANITY_RESOURCE_MANAGER_HPP___

#include "Object.hpp"
#include "String.hpp"
#include <unordered_map>

BEGIN_INANITY

class FileSystem;

/// Абстрактный шаблонный класс менеджера ресурсов.
template <typename Resource>
class ResourceManager : public Object
{
protected:
	/// Файловая система, в которой следует искать ресурсы.
	ptr<FileSystem> fileSystem;

	/// Кэш уже загруженных ресурсов.
	std::unordered_map<String, ptr<Resource> > resources;

protected:
	/// Загрузить ресурс, который ещё не загружен.
	virtual ptr<Resource> Load(const String& resourceName) = 0;

public:
	ResourceManager(ptr<FileSystem> fileSystem);

	/// Получить ресурс.
	ptr<Resource> Get(const String& resourceName);
};

END_INANITY

#endif
