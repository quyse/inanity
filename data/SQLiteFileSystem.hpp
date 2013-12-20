#ifndef ___INANITY_DATA_SQLITEFILESYSTEM_HPP___
#define ___INANITY_DATA_SQLITEFILESYSTEM_HPP___

#include "data.hpp"
#include "../FileSystem.hpp"
#include "../String.hpp"
#include <unordered_map>

BEGIN_INANITY_DATA

class SqliteDb;
class SqliteStatement;

/// Класс файловой системы, основанной на SQLite.
/** Эта файловая система не полностью интегрирована
в Inanity, так как требует прямого доступа к файловой
системе для блокировок и синхронизации. */
class SQLiteFileSystem : public FileSystem
{
private:
	/// Открытая база данных.
	ptr<SqliteDb> db;
	/// Statement для загрузки файла.
	mutable ptr<SqliteStatement> loadFileStmt;
	/// Statement для сохранения файла.
	mutable ptr<SqliteStatement> saveFileStmt;
	/// Statement для получения файлов в каталоге.
	mutable ptr<SqliteStatement> entriesStmt;
	/// Statement для получения всех файлов.
	mutable ptr<SqliteStatement> allEntriesStmt;

	/// Выбросить исключение SQLite.
	void Throw(const char* message) const;

	void ensureLoadFileStmt() const;
	void ensureSaveFileStmt() const;
	void ensureEntriesStmt() const;
	void ensureAllEntriesStmt() const;

	void GetEntries(const String& directoryName, std::vector<String>& entries, bool recursive) const;

public:
	/// Создать или открыть файловую систему в заданном файле.
	SQLiteFileSystem(const String& fileName);

	ptr<File> TryLoadFile(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
	void GetFileNames(std::vector<String>& fileNames) const;
	void GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;
	void GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const;
};

END_INANITY_DATA

#endif
