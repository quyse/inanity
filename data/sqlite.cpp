#include "sqlite.hpp"

BEGIN_INANITY_DATA

SqliteDb::SqliteDb(sqlite3* db)
: db(db) {}

SqliteDb::~SqliteDb()
{
	sqlite3_close(db);
}

SqliteDb::operator sqlite3*() const
{
	return db;
}

SqliteStatement::SqliteStatement(ptr<SqliteDb> db, sqlite3_stmt* stmt)
: db(db), stmt(stmt) {}

SqliteStatement::~SqliteStatement()
{
	sqlite3_finalize(stmt);
}

void SqliteStatement::Reset()
{
	sqlite3_reset(stmt);
	sqlite3_clear_bindings(stmt);
}

SqliteStatement::operator sqlite3_stmt*() const
{
	return stmt;
}

SqliteQuery::SqliteQuery(ptr<SqliteStatement> statement)
: statement(statement) {}

SqliteQuery::~SqliteQuery()
{
	statement->Reset();
}

END_INANITY_DATA
