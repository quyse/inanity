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
	/// Открытая база данных.
	sqlite3* db;
	/// Statement для загрузки файла.
	sqlite3_stmt* loadFileStmt;
	/// Statement для сохранения файла.
	sqlite3_stmt* saveFileStmt;

	// Сбросить запрос и освободить его аргументы.
	static void ResetStmt(sqlite3_stmt* stmt);

	/// Хеш использованных файлов по их данным.
	/** Используется для передачи данных файлов в SQLite,
	и их последующего корректного освобождения. */
	static std::unordered_multimap<void*, ptr<File> > files;
	/// Критическая секция для синхронизации хеша файлов.
	static CriticalSection filesCriticalSection;
	// Захватить файл, чтобы он не освобождался (пока его не освободит SQLite).
	static void AcquireFile(ptr<File> file);
	// Освободить файл по данным.
	static void FreeFile(void* data);

public:
	/// Создать или открыть файловую систему в заданном файле.
	SQLiteFileSystem(const String& fileName);
	~SQLiteFileSystem();

	ptr<File> TryLoadFile(const String& fileName);
	void SaveFile(ptr<File> file, const String& fileName);
};

END_INANITY

#endif
