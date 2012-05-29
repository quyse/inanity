#include "SQLiteFileSystem.hpp"
#include "MemoryFile.hpp"
#include "CriticalCode.hpp"
#include "Exception.hpp"

std::unordered_multimap<void*, ptr<File> > SQLiteFileSystem::files;
CriticalSection SQLiteFileSystem::filesCriticalSection;

SQLiteFileSystem::SQLiteFileSystem(const String& fileName)
{
	try
	{
		if(sqlite3_open(fileName.c_str(), &db) != SQLITE_OK)
		{
			if(db)
				sqlite3_close(db);
			THROW_PRIMARY_EXCEPTION("Can't open db");
		}

		if(sqlite3_exec(db,
			"CREATE TABLE IF NOT EXISTS files ("
			"id INTEGER PRIMARY KEY, "
			"name TEXT NOT NULL UNIQUE COLLATE BINARY, "
			"data BLOB NOT NULL)",
			0, 0, 0) != SQLITE_OK)
		{
			sqlite3_close(db);
			THROW_PRIMARY_EXCEPTION("Can't create table");
		}

		// создать statement для загрузки файла
		if(sqlite3_prepare_v2(db, "SELECT data FROM files WHERE name = ?1", -1, &loadFileStmt, 0) != SQLITE_OK)
		{
			sqlite3_close(db);
			THROW_PRIMARY_EXCEPTION("Can't create load file statement");
		}
		// создать statement для сохранения файла
		if(sqlite3_prepare_v2(db, "INSERT OR REPLACE files (name, data) VALUES (?1, ?2)", -1, &saveFileStmt, 0) != SQLITE_OK)
		{
			sqlite3_finalize(loadFileStmt);
			sqlite3_close(db);
			THROW_PRIMARY_EXCEPTION("Can't create insert file statement");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't open SQLite file system in " + fileName, exception);
	}
}

SQLiteFileSystem::~SQLiteFileSystem()
{
	sqlite3_finalize(loadFileStmt);
	sqlite3_finalize(saveFileStmt);
}

void SQLiteFileSystem::ResetStmt(sqlite3_stmt* stmt)
{
	sqlite3_reset(stmt);
	sqlite3_clear_bindings(stmt);
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
		// установить имя файла в запросе
		if(sqlite3_bind_text(loadFileStmt, 1, fileName.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't bind name parameter");

		// выполнить запрос
		switch(sqlite3_step(loadFileStmt))
		{
		case SQLITE_ROW: // файл найден
			{
				// получить указатель на данные
				const void* fileData = sqlite3_column_blob(loadFileStmt, 1);
				if(!fileData)
				{
					ResetStmt(loadFileStmt);
					THROW_PRIMARY_EXCEPTION("Can't get file data");
				}
				size_t fileSize = sqlite3_column_bytes(loadFileStmt, 1);
				// скопировать данные в память
				ptr<MemoryFile> file = NEW(MemoryFile(fileSize));
				memcpy(file->GetData(), fileData, fileSize);
				// освободить запрос
				ResetStmt(loadFileStmt);
				// и вернуть файл
				return file;
			}
		case SQLITE_DONE: // файл не найден
			ResetStmt(loadFileStmt);
			return nullptr;
		default:
			ResetStmt(loadFileStmt);
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
		// установить имя файла и данные в запросе
		if(sqlite3_bind_text(saveFileStmt, 1, fileName.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
			THROW_PRIMARY_EXCEPTION("Can't bind name parameter");
		AcquireFile(file);
		if(sqlite3_bind_blob(saveFileStmt, 2, file->GetData(), file->GetSize(), FreeFile) != SQLITE_OK)
		{
			ResetStmt(saveFileStmt);
			THROW_PRIMARY_EXCEPTION("Can't bind data parameter");
		}

		// выполнить запрос
		int result = sqlite3_step(saveFileStmt);

		// очистка в любом случае
		ResetStmt(saveFileStmt);

		// если была ошибка
		if(result != SQLITE_DONE)
			THROW_PRIMARY_EXCEPTION("Can't execute statement");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't save file to SQLite file system", exception);
	}
}
