#include "SQLiteFileSystem.hpp"
#include "MemoryFile.hpp"
#include "CriticalCode.hpp"
#include "Exception.hpp"
#include <memory.h>

/* Реализация в базе SQLite файловой системы.
Структура базы описана в конструкторе.
Сейчас операции выполняются эффективно, кроме GetDirectoryEntries -
она выполняется за количество всех файлов в каталоге (рекурсивное количество),
хотя возвращает корректно файлы и каталоги только из заданного каталога.
Вызвано это тем, что в базе не хранятся каталоги, они получаются неявно.
*/

std::unordered_multimap<void*, ptr<File> > SQLiteFileSystem::files;
CriticalSection SQLiteFileSystem::filesCriticalSection;

SQLiteFileSystem::Db::Db(sqlite3* db) : db(db) {}
SQLiteFileSystem::Db::~Db()
{
	sqlite3_close(db);
}
SQLiteFileSystem::Db::operator sqlite3*() const
{
	return db;
}

SQLiteFileSystem::Statement::Statement(ptr<Db> db, sqlite3_stmt* stmt) : db(db), stmt(stmt) {}
SQLiteFileSystem::Statement::~Statement()
{
	sqlite3_finalize(stmt);
}
void SQLiteFileSystem::Statement::Reset()
{
	sqlite3_reset(stmt);
	sqlite3_clear_bindings(stmt);
}
SQLiteFileSystem::Statement::operator sqlite3_stmt*() const
{
	return stmt;
}

SQLiteFileSystem::QueryHandle::QueryHandle(ptr<Statement> statement) : statement(statement) {}
SQLiteFileSystem::QueryHandle::~QueryHandle()
{
	statement->Reset();
}

SQLiteFileSystem::SQLiteFileSystem(const String& fileName)
{
	try
	{
		{
			sqlite3* dbPtr;
			if(sqlite3_open(fileName.c_str(), &dbPtr) != SQLITE_OK)
			{
				if(dbPtr)
					sqlite3_close(dbPtr);
				THROW_PRIMARY_EXCEPTION("Can't open db");
			}
			db = NEW(Db(dbPtr));
		}

		if(sqlite3_exec(*db,
			"CREATE TABLE IF NOT EXISTS files ("
			"id INTEGER PRIMARY KEY, "
			"name TEXT NOT NULL UNIQUE COLLATE BINARY, "
			"data BLOB NOT NULL)",
			0, 0, 0) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't create table");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't open SQLite file system in " + fileName, exception);
	}
}

void SQLiteFileSystem::ensureLoadFileStmt() const
{
	if(!loadFileStmt)
	{
		sqlite3_stmt* stmtPtr;
		if(sqlite3_prepare_v2(*db, "SELECT data FROM files WHERE name = ?1", -1, &stmtPtr, 0) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't create load file statement");
		loadFileStmt = NEW(Statement(db, stmtPtr));
	}
}

void SQLiteFileSystem::ensureSaveFileStmt() const
{
	if(!saveFileStmt)
	{
		sqlite3_stmt* stmtPtr;
		if(sqlite3_prepare_v2(*db, "INSERT OR REPLACE files (name, data) VALUES (?1, ?2)", -1, &stmtPtr, 0) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't create save file statement");
		saveFileStmt = NEW(Statement(db, stmtPtr));
	}
}

void SQLiteFileSystem::ensureEntriesStmt() const
{
	if(!entriesStmt)
	{
		sqlite3_stmt* stmtPtr;
		if(sqlite3_prepare_v2(*db, "SELECT name FROM files WHERE name LIKE ?1 ORDER BY name ASC", -1, &stmtPtr, 0) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't create entries file statement");
		entriesStmt = NEW(Statement(db, stmtPtr));
	}
}

void SQLiteFileSystem::ensureAllEntriesStmt() const
{
	if(!allEntriesStmt)
	{
		sqlite3_stmt* stmtPtr;
		if(sqlite3_prepare_v2(*db, "SELECT name FROM files ORDER BY name ASC", -1, &stmtPtr, 0) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't create all entries file statement");
		allEntriesStmt = NEW(Statement(db, stmtPtr));
	}
}

void SQLiteFileSystem::AcquireFile(ptr<File> file)
{
	CriticalCode code(filesCriticalSection);
	files.insert(std::make_pair(file->GetData(), file));
}

void SQLiteFileSystem::FreeFile(void* data)
{
	CriticalCode code(filesCriticalSection);
	std::unordered_multimap<void*, ptr<File> >::iterator i = files.find(data);
	if(i == files.end())
		THROW_PRIMARY_EXCEPTION("Invalid call of SQLiteFileSystem::FreeFile");
	files.erase(i);
}

ptr<File> SQLiteFileSystem::TryLoadFile(const String& fileName)
{
	try
	{
		ensureLoadFileStmt();

		QueryHandle query(loadFileStmt);
		// установить имя файла в запросе
		if(sqlite3_bind_text(*loadFileStmt, 1, fileName.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't bind name parameter");

		// выполнить запрос
		switch(sqlite3_step(*loadFileStmt))
		{
		case SQLITE_ROW: // файл найден
			{
				// получить указатель на данные
				const void* fileData = sqlite3_column_blob(*loadFileStmt, 1);
				if(!fileData)
					THROW_PRIMARY_EXCEPTION("Can't get file data");
				size_t fileSize = sqlite3_column_bytes(*loadFileStmt, 1);
				// скопировать данные в память
				ptr<MemoryFile> file = NEW(MemoryFile(fileSize));
				memcpy(file->GetData(), fileData, fileSize);
				// и вернуть файл
				return file;
			}
		case SQLITE_DONE: // файл не найден
			return nullptr;
		default:
			THROW_PRIMARY_EXCEPTION("Error with statement step");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't try to load file from SQLite file system", exception);
	}
}

void SQLiteFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	try
	{
		ensureSaveFileStmt();

		QueryHandle query(saveFileStmt);
		// установить имя файла и данные в запросе
		if(sqlite3_bind_text(*saveFileStmt, 1, fileName.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't bind name parameter");
		AcquireFile(file);
		if(sqlite3_bind_blob(*saveFileStmt, 2, file->GetData(), file->GetSize(), FreeFile) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't bind data parameter");

		// выполнить запрос
		int result = sqlite3_step(*saveFileStmt);

		// если была ошибка
		if(result != SQLITE_DONE)
			THROW_PRIMARY_EXCEPTION("Can't execute statement");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't save file to SQLite file system", exception);
	}
}

void SQLiteFileSystem::GetEntries(const String& directoryName, std::vector<String>& entries, bool recursive) const
{
	try
	{
		ensureEntriesStmt();

		QueryHandle query(entriesStmt);

		// установить имя каталога в запросе
		if(sqlite3_bind_text(*entriesStmt, 1, (directoryName + "%").c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't bind name parameter");

		// запомнить границу, с которой начались наши результаты
		size_t size1 = entries.size();

		// выполнять запрос, пока возвращаются результаты
		for(;;)
		{
			int result = sqlite3_step(*entriesStmt);
			if(result == SQLITE_DONE)
				break;
			if(result == SQLITE_ROW)
			{
				// получить имя файла
				String fileName = (const char*)sqlite3_column_text(*entriesStmt, 0);
#ifdef _DEBUG
				// проверить, что оно начинается с имени каталога
				if(fileName.length() < directoryName.length() || memcmp(fileName.c_str(), directoryName.c_str(), directoryName.length()) != 0)
					THROW_PRIMARY_EXCEPTION("File name is not begins with directory name");
#endif
				// отрезать от него имя каталога
				fileName = fileName.substr(directoryName.length());
				// если требуется нерекурсивно
				if(!recursive)
				{
					// проверить, что этот файл лежит именно в этом каталоге, а не в каком-то вложенном
					// вернее, просто отделить всё, что лежит после первого слеша
					// таким образом мы добавим каталоги
					size_t slashPos = fileName.find_first_of('/');
					if(slashPos != String::npos)
						fileName = fileName.substr(0, slashPos + 1);
				}
				// проверить, что это имя не было добавлено ранее
				if(recursive || entries.size() <= size1 || entries.back() != fileName)
					// и добавить
					entries.push_back(fileName);
			}
			else
				THROW_PRIMARY_EXCEPTION("Can't execute statement");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get SQLite file system directory entries for " + directoryName, exception);
	}
}

void SQLiteFileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	try
	{
		ensureAllEntriesStmt();

		QueryHandle query(allEntriesStmt);

		// выполнять запрос, пока возвращаются результаты
		for(;;)
		{
			int result = sqlite3_step(*allEntriesStmt);
			if(result == SQLITE_DONE)
				break;
			if(result == SQLITE_ROW)
				// получить имя файла и добавить в список
				fileNames.push_back((const char*)sqlite3_column_text(*entriesStmt, 0));
			else
				THROW_PRIMARY_EXCEPTION("Can't execute statement");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get SQLite file system file names", exception);
	}
}

void SQLiteFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	GetEntries(directoryName, entries, false);
}

void SQLiteFileSystem::GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	GetEntries(directoryName, entries, true);
}
