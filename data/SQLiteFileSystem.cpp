#include "SQLiteFileSystem.hpp"
#include "sqlite.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include <memory.h>

BEGIN_INANITY_DATA

/* Реализация в базе SQLite файловой системы.
Структура базы описана в конструкторе.
Сейчас операции выполняются эффективно, кроме GetDirectoryEntries -
она выполняется за количество всех файлов в каталоге (рекурсивное количество),
хотя возвращает корректно файлы и каталоги только из заданного каталога.
Вызвано это тем, что в базе не хранятся каталоги, они получаются неявно.
*/

SQLiteFileSystem::SQLiteFileSystem(const String& fileName)
{
	try
	{
		db = SqliteDb::Open(fileName.c_str());

		if(sqlite3_exec(*db,
			"CREATE TABLE IF NOT EXISTS files ("
			"id INTEGER PRIMARY KEY, "
			"name TEXT NOT NULL UNIQUE COLLATE BINARY, "
			"data BLOB NOT NULL)",
			0, 0, 0) != SQLITE_OK)
			Throw("Can't create table");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't open SQLite file system in " + fileName, exception);
	}
}

void SQLiteFileSystem::Throw(const char* message) const
{
	THROW_SECONDARY(message, db->Error());
}

void SQLiteFileSystem::ensureLoadFileStmt() const
{
	if(!loadFileStmt)
		loadFileStmt = db->CreateStatement("SELECT data FROM files WHERE name = ?1");
}

void SQLiteFileSystem::ensureSaveFileStmt() const
{
	if(!saveFileStmt)
		saveFileStmt = db->CreateStatement("INSERT OR REPLACE INTO files (name, data) VALUES (?1, ?2)");
}

void SQLiteFileSystem::ensureEntriesStmt() const
{
	if(!entriesStmt)
		entriesStmt = db->CreateStatement("SELECT name FROM files WHERE name LIKE ?1 ORDER BY name ASC");
}

void SQLiteFileSystem::ensureAllEntriesStmt() const
{
	if(!allEntriesStmt)
		allEntriesStmt = db->CreateStatement("SELECT name FROM files ORDER BY name ASC");
}

ptr<File> SQLiteFileSystem::TryLoadFile(const String& fileName)
{
	try
	{
		ensureLoadFileStmt();

		SqliteQuery query(loadFileStmt);
		// установить имя файла в запросе
		loadFileStmt->Bind(1, fileName);

		// выполнить запрос
		switch(loadFileStmt->Step())
		{
		case SQLITE_ROW: // файл найден
			return loadFileStmt->ColumnBlob(0);
		case SQLITE_DONE: // файл не найден
			return nullptr;
		default:
			Throw("Error with statement step");
			return 0;
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't try to load file from SQLite file system", exception);
	}
}

void SQLiteFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	try
	{
		ensureSaveFileStmt();

		SqliteQuery query(saveFileStmt);
		// установить имя файла и данные в запросе
		saveFileStmt->Bind(1, fileName);
		saveFileStmt->Bind(2, file);

		// выполнить запрос
		int result = saveFileStmt->Step();

		// если была ошибка
		if(result != SQLITE_DONE)
			Throw("Can't execute statement");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't save file to SQLite file system", exception);
	}
}

void SQLiteFileSystem::GetEntries(const String& directoryName, std::vector<String>& entries, bool recursive) const
{
	try
	{
		ensureEntriesStmt();

		SqliteQuery query(entriesStmt);

		// установить имя каталога в запросе
		entriesStmt->Bind(1, directoryName + "%");

		// запомнить границу, с которой начались наши результаты
		size_t size1 = entries.size();

		// выполнять запрос, пока возвращаются результаты
		for(;;)
		{
			int result = entriesStmt->Step();
			if(result == SQLITE_DONE)
				break;
			if(result == SQLITE_ROW)
			{
				// получить имя файла
				String fileName = entriesStmt->ColumnText(0);
#ifdef _DEBUG
				// проверить, что оно начинается с имени каталога
				if(fileName.length() < directoryName.length() || memcmp(fileName.c_str(), directoryName.c_str(), directoryName.length()) != 0)
					THROW("File name is not begins with directory name");
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
				Throw("Can't execute statement");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't get SQLite file system directory entries for " + directoryName, exception);
	}
}

void SQLiteFileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	try
	{
		ensureAllEntriesStmt();

		SqliteQuery query(allEntriesStmt);

		// выполнять запрос, пока возвращаются результаты
		for(;;)
		{
			int result = allEntriesStmt->Step();
			if(result == SQLITE_DONE)
				break;
			if(result == SQLITE_ROW)
				// получить имя файла и добавить в список
				fileNames.push_back(allEntriesStmt->ColumnText(0));
			else
				Throw("Can't execute statement");
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't get SQLite file system file names", exception);
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

END_INANITY_DATA
