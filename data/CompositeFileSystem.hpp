#ifndef ___INANITY_DATA_COMPOSITE_FILE_SYSTEM_H___
#define ___INANITY_DATA_COMPOSITE_FILE_SYSTEM_H___

#include "data.hpp"
#include "../FileSystem.hpp"
#include <vector>

BEGIN_INANITY_DATA

/// Класс составной файловой системы.
/** Позволяет объединять несколько файловых систем.
Файловые системы можно монтировать в различные каталоги, указывая префикс пути,
который должен быть у имени файла.
Файл открывается той файловой системой, имеющей подходящий префикс.
Если таких несколько, файл открывается той системой, которая выше по списку. */
class CompositeFileSystem : public FileSystem
{
private:
	/// Список файловых систем с путями монтирования.
	/** Путь монтирования должен начинаться и заканчиваться на слеш /. */
	std::vector<std::pair<String, ptr<FileSystem> > > fileSystems;

	template <typename F>
	auto WithPath(const String& path, F&& f) const -> decltype(f(ptr<FileSystem>(), String()));

	void NormalizePath(String& path);

public:
	/// Примонтировать файловую систему в указанный путь.
	/** Добавляет систему в конец списка. */
	void Mount(ptr<FileSystem> fileSystem, String path = "/");
	/// Примонтировать файловую систему в указанный путь.
	/** Заменяет существующую систему, если есть, или добавляет в конец списка. */
	void Remount(ptr<FileSystem> fileSystem, String path = "/");

	ptr<File> TryLoadFile(const String& fileName) override;
	ptr<Storage> LoadStorage(const String& fileName) override;
	ptr<InputStream> LoadStream(const String& fileName) override;
	void SaveFile(ptr<File> file, const String& fileName) override;
	ptr<OutputStream> SaveStream(const String& fileName) override;
	time_t GetFileMTime(const String& fileName) override;
	void GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const override;
	void MakeDirectory(const String& directoryName) override;
	void DeleteEntry(const String& entryName) override;
	EntryType GetEntryType(const String& entryName) const override;
};

END_INANITY_DATA

#endif
