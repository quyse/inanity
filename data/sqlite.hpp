#ifndef ___INANITY_DATA_SQLITE_HPP___
#define ___INANITY_DATA_SQLITE_HPP___

#include "data.hpp"
#include "../deps/sqlite/sqlite3.h"
#include "../CriticalSection.hpp"
#include "../String.hpp"
#include <unordered_map>
#include <iostream>

BEGIN_INANITY

class File;
class Exception;

END_INANITY

BEGIN_INANITY_DATA

class SqliteStatement;
class SqliteTransaction;

/// Class incapsulating sqlite3.
class SqliteDb : public Object
{
	friend class SqliteTransaction;
private:
	sqlite3* db;

	/// Static cache of files which transferred to SQLite.
	/** We need it static because SQLite uses simple function pointers
	as destructors without any 'context' parameter. */
	typedef std::unordered_multimap<void*, ptr<File> > Files;
	static Files files;
	static CriticalSection csFiles;

	sqlite3_stmt* stmtSavepoint;
	sqlite3_stmt* stmtRelease;
	sqlite3_stmt* stmtRollbackTo;

public:
	SqliteDb(sqlite3* db);
	~SqliteDb();

	static ptr<SqliteDb> Open(const char* fileName);

	operator sqlite3*() const;

	static void AcquireFile(ptr<File> file);
	static void FreeFile(void* data);

	ptr<SqliteStatement> CreateStatement(const char* query);

	long long LastInsertRowId() const;

	/// Get exception corresponding to SQLite error.
	ptr<Exception> Error() const;

	/// Perform VACUUM.
	void Vacuum();
	/// Perform PRAGMA integrity_check.
	void IntegrityCheck(std::ostream& stream);
};

/// Class incapsulating sqlite3_stmt.
class SqliteStatement : public Object
{
private:
	ptr<SqliteDb> db;
	sqlite3_stmt* stmt;

public:
	SqliteStatement(ptr<SqliteDb> db, sqlite3_stmt* stmt);
	~SqliteStatement();

	// Fields numeration starts from one.
	void Bind(int number, const char* value);
	void Bind(int number, const String& value);
	void Bind(int number, int value);
	void Bind(int number, long long value);
	void Bind(int number, const void* data, size_t size);
	void Bind(int number, ptr<File> value);
	void BindNull(int number);

	// Columns numeration starts from zero.
	String ColumnText(int number) const;
	int ColumnInt(int number) const;
	long long ColumnInt64(int number) const;
	ptr<File> ColumnBlob(int number) const;
	size_t ColumnBlobSize(int number) const;

	int Step();

	void Reset();
	operator sqlite3_stmt*() const;
};

/// This class just reset sqlite3_stmt in destructor.
class SqliteQuery
{
private:
	ptr<SqliteStatement> statement;

public:
	SqliteQuery(ptr<SqliteStatement> statement);
	~SqliteQuery();
};

/// SQLite transaction wrapper.
/** Automatically rollbacks transaction
if isn't commited explicitly. May be nested. */
class SqliteTransaction
{
private:
	ptr<SqliteDb> db;
	bool finished;

public:
	SqliteTransaction(ptr<SqliteDb> db);
	~SqliteTransaction();

	void Commit();
};

END_INANITY_DATA

#endif
