#ifndef ___INANITY_SQLITEFILESYSTEM_HPP___
#define ___INANITY_SQLITEFILESYSTEM_HPP___

#include "FileSystem.hpp"
#include "String.hpp"
#include "CriticalSection.hpp"
#include "deps/sqlite/sqlite3.h"
#include <unordered_map>

BEGIN_INANITY

class File;

/// Класс файловой системы, основанной на SQLite.
/** Эта файловая система не полностью интегрирована
в Inanity, так как требует прямого доступа к файловой
системе для блокировок и синхронизации. */
class SQLiteFileSystem : public FileSystem
{
private:
	/// Класс, инкапсулирующий объект sqlite3.
	class Db : public Object
	{
	private:
		sqlite3* db;

	public:
		Db(sqlite3* db);
		~Db();

		operator sqlite3*() const;
	};

	/// Класс, инкапсулирующий объект sqlite3_stmt.
	class Statement : public Object
	{
	private:
		ptr<Db> db;
		sqlite3_stmt* stmt;

	public:
		Statement(ptr<Db> db, sqlite3_stmt* stmt);
		~Statement();

		void Reset();
		operator sqlite3_stmt*() const;
	};

	/// Класс, выполняющий сброс объекта sqlite3_stmt в деструкторе.
	class QueryHandle
	{
	private:
		ptr<Statement> statement;

	public:
		QueryHandle(ptr<Statement> statement);
		~QueryHandle();
	};

	/// Открытая база данных.
	ptr<Db> db;
	/// Statement для загрузки файла.
	mutable ptr<Statement> loadFileStmt;
	/// Statement для сохранения файла.
	mutable ptr<Statement> saveFileStmt;
	/// Statement для получения файлов в каталоге.
	mutable ptr<Statement> entriesStmt;
	/// Statement для получения всех файлов.
	mutable ptr<Statement> allEntriesStmt;

	/// Выбросить исключение SQLite.
	void Throw(const char* message) const;

	void ensureLoadFileStmt() const;
	void ensureSaveFileStmt() const;
	void ensureEntriesStmt() const;
	void ensureAllEntriesStmt() const;

	/// Хеш использованных файлов по их данным.
	/** Используется для передачи данных файлов в SQLite,
	и их последующего корректного освобождения. */
	static std::unordered_multimap<void*, ptr<File> > files;
	/// Критическая секция для синхронизации хеша файлов.
	static CriticalSection filesCriticalSection;
	/// Захватить файл, чтобы он не освобождался (пока его не освободит SQLite).
	static void AcquireFile(ptr<File> file);
	/// Освободить файл по данным.
	static void FreeFile(void* data);

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

END_INANITY

#endif
