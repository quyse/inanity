#ifndef ___INANITY_RESOURCE_STORE_HPP___
#define ___INANITY_RESOURCE_STORE_HPP___

#include "Object.hpp"
#include "String.hpp"
#include "File.hpp"
#include "FileSystem.hpp"
#include "Exception.hpp"
#include <hash_map>

BEGIN_INANITY

/// Класс хранилища ресурсов.
/** Предоставляет доступ к загрузке ресурсов из указанной файловой системы. */
class ResourceStore : public Object
{
private:
	/// Файловая система.
	ptr<FileSystem> fileSystem;

public:
	ResourceStore(ptr<FileSystem> fileSystem);

	/// Загрузить ресурс.
	template <typename T>
	ptr<T> LoadResource(const String& fileName)
	{
		try
		{
			return T::Deserialize(fileSystem->LoadFile(fileName));
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't load resource: " + fileName, exception);
		}
	}
};

END_INANITY

#endif
