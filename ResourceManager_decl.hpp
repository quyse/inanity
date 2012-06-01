#ifndef ___INANITY_RESOURCE_MANAGER_DECL_HPP___
#define ___INANITY_RESOURCE_MANAGER_DECL_HPP___

#include "String.hpp"
#include <unordered_map>

BEGIN_INANITY

class FileSystem;

/// Класс менеджера ресурсов.
/** Загружает ресурсы, и кэширует их для исключения повторной загрузки.
Класс ресурса T должен иметь метод десериализации:
static ptr<T> T::Deserialize(ptr<File> file, ResourceLoader* resourceLoader);

Кроме нормальных ресурсов, есть статические ресурсы - это ресурсы, которые
есть только в одном экземпляре (в данном ресурсном менеджере).
Класс статического ресурса T должен иметь специальное поле с именем ресурса:
static String T::staticResourceName;
Такие ресурсы обычно не загружаются из файлов. Вместо этого они просто как-то создаются.
Для удобной работы с ними предусмотрены специальные методы: AddStaticResource и GetStaticResource.
Также класс статического ресурса может иметь метод:
static ptr<T> CreateStaticResource(ResourceLoader* resourceLoader);
Это позволит ему создаваться простым вызовом ResourceManager::CreateStaticResource<T>().
*/
class ResourceManager : public Object
{
private:
	/// Файловая система, из которой берутся ресурсы.
	ptr<FileSystem> fileSystem;
	/// Карта уже загруженных ресурсов.
	std::unordered_map<String, ptr<Object> > resources;

	/// Получить префикс для имени файла.
	String GetFileNamePrefix(const String& fileName);

public:
	ResourceManager(ptr<FileSystem> fileSystem);

	/// Добавить загруженный ресурс.
	/** Позволяет добавлять в карту загруженных ресурсов ресурсы, которые уже как-то
	загружены в память (например, не из файла). */
	template <typename T>
	void AddResource(const String& fileName, ptr<T> resource);

	/// Получить ресурс без загрузки.
	/** Если ресурса нет, выбрасывается исключение.
	Метод предназначен для ресурсов, которые не загружаются из файлов, а добавляются с помощью AddResource. */
	template <typename T>
	ptr<T> GetResource(const String& fileName) const;

	/// Получить ресурс, если он уже загружен; загрузить ресурс из файла, если ещё нет.
	template <typename T>
	ptr<T> LoadResource(const String& fileName);

	/// Добавить статический ресурс.
	template <typename T>
	void AddStaticResource(ptr<T> resource);

	/// Получить статический ресурс.
	template <typename T>
	ptr<T> GetStaticResource() const;

	/// Создать статический ресурс.
	/** Если статический ресурс поддерживает метод CreateStaticResource,
	то можно легко создать его с помощью данного метода. */
	template <typename T>
	void CreateStaticResource();

	/// Загрузить статический ресурс.
	/** То есть получить его, а если его ещё нет - создать. */
	template <typename T>
	ptr<T> LoadStaticResource();
};

END_INANITY

#endif
