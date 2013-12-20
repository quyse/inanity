#include "sqlite.hpp"
#include "../File.hpp"
#include "../CriticalCode.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_DATA

//*** SqliteDb

SqliteDb::Files SqliteDb::files;
CriticalSection SqliteDb::csFiles;

SqliteDb::SqliteDb(sqlite3* db) :
	db(db),
	stmtBeginTransaction(nullptr),
	stmtCommitTransaction(nullptr),
	stmtRollbackTransaction(nullptr)
{}

SqliteDb::~SqliteDb()
{
	if(stmtBeginTransaction)
		sqlite3_finalize(stmtBeginTransaction);
	if(stmtCommitTransaction)
		sqlite3_finalize(stmtCommitTransaction);
	if(stmtRollbackTransaction)
		sqlite3_finalize(stmtRollbackTransaction);

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

void SqliteStatement::Bind(int number, ptr<File> value)
{
	SqliteDb::AcquireFile(value);
	if(sqlite3_bind_blob(stmt, number, value->GetData(), (int)value->GetSize(), &SqliteDb::FreeFile) != SQLITE_OK)
		THROW_SECONDARY("Can't bind blob to SQLite statement", db->Error());
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

ptr<File> SqliteStatement::ColumnBlob(int number) const
{
	int size = sqlite3_column_bytes(stmt, number);
	ptr<MemoryFile> file = NEW(MemoryFile(size));
	memcpy(file->GetData(), sqlite3_column_blob(stmt, number), size);
	return file;
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
	if(!db->stmtBeginTransaction)
	{
		if(sqlite3_prepare_v2(*db, "BEGIN TRANSACTION", -1, &db->stmtBeginTransaction, nullptr) != SQLITE_OK)
			THROW_SECONDARY("Can't create SQLite begin transaction statement", db->Error());
		if(sqlite3_prepare_v2(*db, "COMMIT TRANSACTION", -1, &db->stmtCommitTransaction, nullptr) != SQLITE_OK)
			THROW_SECONDARY("Can't create SQLite commit transaction statement", db->Error());
		if(sqlite3_prepare_v2(*db, "ROLLBACK TRANSACTION", -1, &db->stmtRollbackTransaction, nullptr) != SQLITE_OK)
			THROW_SECONDARY("Can't create SQLite rollback transaction statement", db->Error());
	}

	sqlite3_reset(db->stmtBeginTransaction);
	if(sqlite3_step(db->stmtBeginTransaction) != SQLITE_DONE)
		THROW_SECONDARY("Can't begin SQLite transaction", db->Error());
}

SqliteTransaction::~SqliteTransaction()
{
	if(finished)
		return;

	sqlite3_reset(db->stmtRollbackTransaction);
	sqlite3_step(db->stmtRollbackTransaction);
}

void SqliteTransaction::Commit()
{
	if(finished)
		THROW("Can't commit finished transaction");

	sqlite3_reset(db->stmtCommitTransaction);
	if(sqlite3_step(db->stmtCommitTransaction) != SQLITE_DONE)
		THROW_SECONDARY("Can't commit SQLite transaction", db->Error());

	finished = true;
}

END_INANITY_DATA
