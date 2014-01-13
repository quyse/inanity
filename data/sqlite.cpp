#include "sqlite.hpp"
#include "../File.hpp"
#include "../CriticalCode.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include <cstring>

BEGIN_INANITY_DATA

//*** SqliteDb

SqliteDb::Files SqliteDb::files;
CriticalSection SqliteDb::csFiles;

SqliteDb::SqliteDb(sqlite3* db) :
	db(db),
	stmtSavepoint(nullptr),
	stmtRelease(nullptr),
	stmtRollbackTo(nullptr)
{}

SqliteDb::~SqliteDb()
{
	if(stmtSavepoint)
		sqlite3_finalize(stmtSavepoint);
	if(stmtRelease)
		sqlite3_finalize(stmtRelease);
	if(stmtRollbackTo)
		sqlite3_finalize(stmtRollbackTo);

	sqlite3_close(db);
}

ptr<SqliteDb> SqliteDb::Open(const char* fileName)
{
	sqlite3* db;
	if(sqlite3_open(fileName, &db) != SQLITE_OK)
	{
		if(db)
			sqlite3_close(db);
		THROW("Can't open SQLite database");
	}

	return NEW(SqliteDb(db));
}


SqliteDb::operator sqlite3*() const
{
	return db;
}

void SqliteDb::AcquireFile(ptr<File> file)
{
	CriticalCode cc(csFiles);
	files.insert(std::make_pair(file->GetData(), file));
}

void SqliteDb::FreeFile(void* data)
{
	CriticalCode cc(csFiles);
	Files::iterator i = files.find(data);
	if(i == files.end())
		THROW("Invalid call of SqliteDb::FreeFile");
	files.erase(i);
}

ptr<SqliteStatement> SqliteDb::CreateStatement(const char* query)
{
	sqlite3_stmt* stmt;
	if(sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
		THROW_SECONDARY("Can't create SQLite statement", Error());
	return NEW(SqliteStatement(this, stmt));
}

long long SqliteDb::LastInsertRowId() const
{
	return sqlite3_last_insert_rowid(db);
}

ptr<Exception> SqliteDb::Error() const
{
	return NEW(Exception(sqlite3_errmsg(db)));
}

//*** SqliteStatement

SqliteStatement::SqliteStatement(ptr<SqliteDb> db, sqlite3_stmt* stmt)
: db(db), stmt(stmt) {}

SqliteStatement::~SqliteStatement()
{
	sqlite3_finalize(stmt);
}

void SqliteStatement::Bind(int number, const char* value)
{
	if(sqlite3_bind_text(stmt, number, value, -1, SQLITE_TRANSIENT) != SQLITE_OK)
		THROW_SECONDARY("Can't bind text to SQLite statement", db->Error());
}

void SqliteStatement::Bind(int number, const String& value)
{
	Bind(number, value.c_str());
}

void SqliteStatement::Bind(int number, int value)
{
	if(sqlite3_bind_int(stmt, number, value) != SQLITE_OK)
		THROW_SECONDARY("Can't bind int to SQLite statement", db->Error());
}

void SqliteStatement::Bind(int number, long long value)
{
	if(sqlite3_bind_int64(stmt, number, value) != SQLITE_OK)
		THROW_SECONDARY("Can't bind int64 to SQLite statement", db->Error());
}

void SqliteStatement::Bind(int number, const void* data, size_t size)
{
	if(sqlite3_bind_blob(stmt, number, data, (int)size, SQLITE_TRANSIENT) != SQLITE_OK)
		THROW_SECONDARY("Can't bind blob data to SqliteStatement", db->Error());
}

void SqliteStatement::Bind(int number, ptr<File> value)
{
	SqliteDb::AcquireFile(value);
	if(sqlite3_bind_blob(stmt, number, value->GetData(), (int)value->GetSize(), &SqliteDb::FreeFile) != SQLITE_OK)
		THROW_SECONDARY("Can't bind blob file to SQLite statement", db->Error());
}

void SqliteStatement::BindNull(int number)
{
	if(sqlite3_bind_null(stmt, number) != SQLITE_OK)
		THROW_SECONDARY("Can't bind null to SQLite statement", db->Error());
}

String SqliteStatement::ColumnText(int number) const
{
	return (const char*)sqlite3_column_text(stmt, number);
}

int SqliteStatement::ColumnInt(int number) const
{
	return sqlite3_column_int(stmt, number);
}

long long SqliteStatement::ColumnInt64(int number) const
{
	return sqlite3_column_int64(stmt, number);
}

ptr<File> SqliteStatement::ColumnBlob(int number) const
{
	int size = sqlite3_column_bytes(stmt, number);
	ptr<MemoryFile> file = NEW(MemoryFile(size));
	memcpy(file->GetData(), sqlite3_column_blob(stmt, number), size);
	return file;
}

size_t SqliteStatement::ColumnBlobSize(int number) const
{
	return sqlite3_column_bytes(stmt, number);
}

int SqliteStatement::Step()
{
	return sqlite3_step(stmt);
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

//*** SqliteQuery

SqliteQuery::SqliteQuery(ptr<SqliteStatement> statement)
: statement(statement) {}

SqliteQuery::~SqliteQuery()
{
	statement->Reset();
}

//*** SqliteTransaction

SqliteTransaction::SqliteTransaction(ptr<SqliteDb> db)
: db(db), finished(false)
{
	if(!db->stmtSavepoint)
	{
		if(sqlite3_prepare_v2(*db, "SAVEPOINT T", -1, &db->stmtSavepoint, nullptr) != SQLITE_OK)
			THROW_SECONDARY("Can't create SQLite SAVEPOINT statement", db->Error());
		if(sqlite3_prepare_v2(*db, "RELEASE T", -1, &db->stmtRelease, nullptr) != SQLITE_OK)
			THROW_SECONDARY("Can't create SQLite RELEASE statement", db->Error());
		if(sqlite3_prepare_v2(*db, "ROLLBACK TO T", -1, &db->stmtRollbackTo, nullptr) != SQLITE_OK)
			THROW_SECONDARY("Can't create SQLite ROLLBACK TO statement", db->Error());
	}

	sqlite3_reset(db->stmtSavepoint);
	if(sqlite3_step(db->stmtSavepoint) != SQLITE_DONE)
		THROW_SECONDARY("Can't make SQLite savepoint", db->Error());
}

SqliteTransaction::~SqliteTransaction()
{
	if(finished)
		return;

	sqlite3_reset(db->stmtRollbackTo);
	sqlite3_step(db->stmtRollbackTo);
	sqlite3_reset(db->stmtRelease);
	sqlite3_step(db->stmtRelease);
}

void SqliteTransaction::Commit()
{
	if(finished)
		THROW("Can't commit finished transaction");

	sqlite3_reset(db->stmtRelease);
	if(sqlite3_step(db->stmtRelease) != SQLITE_DONE)
		THROW_SECONDARY("Can't release savepoint", db->Error());

	finished = true;
}

END_INANITY_DATA
