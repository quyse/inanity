#ifndef ___INANITY_DATA_SQLITE_HPP___
#define ___INANITY_DATA_SQLITE_HPP___

#include "data.hpp"
#include "../deps/sqlite/sqlite3.h"

BEGIN_INANITY_DATA

/// Class incapsulating sqlite3.
class SqliteDb : public Object
{
private:
	sqlite3* db;

public:
	SqliteDb(sqlite3* db);
	~SqliteDb();

	operator sqlite3*() const;
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

END_INANITY_DATA

#endif
