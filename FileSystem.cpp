#include "FileSystem.hpp"
#include "File.hpp"
#include "FileInputStream.hpp"
#include "Exception.hpp"

ptr<File> FileSystem::LoadFile(const String& fileName)
{
	ptr<File> file = TryLoadFile(fileName);
	if(file)
		return file;
	THROW_PRIMARY_EXCEPTION("Can't load file " + fileName);
}

ptr<File> FileSystem::TryLoadFile(const String& fileName)
{
	try
	{
		return LoadFile(fileName);
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		return 0;
	}
}

ptr<InputStream> FileSystem::LoadFileAsStream(const String& fileName)
{
	try
	{
		return NEW(FileInputStream(LoadFile(fileName)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load file " + fileName + " as stream", exception);
	}
}

void FileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	THROW_PRIMARY_EXCEPTION("Saving files in this filesystem is not supported");
}

ptr<OutputStream> FileSystem::SaveFileAsStream(const String& fileName)
{
	THROW_PRIMARY_EXCEPTION("Saving files as stream in this filesystem is not supported");
}

void FileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	THROW_PRIMARY_EXCEPTION("Getting file names in this filesystem is not supported");
}

void FileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	THROW_PRIMARY_EXCEPTION("Getting directory entries in this filesystem is not supported");
}

void FileSystem::GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	// получить (нерекурсивно) файлы и каталоги в заданном каталоге
	size_t size1 = entries.size();
	GetDirectoryEntries(directoryName, entries);
	size_t size2 = entries.size();

	// перебрать полученные файлы и каталоги
	for(size_t i = size1; i < size2; ++i)
	{
		// получить файл
		String& entry = entries[i];
		// добавить к имени имя текущего каталога, чтобы сделать абсолютные имена
		entry = directoryName + entry;
		// если это каталог
		if(entry.length() && entry[entry.length() - 1] == '/')
			// рекурсивно получить файлы и каталоги в нём
			// копия имени делается, так как ссылка может стать недействительной
			GetAllDirectoryEntries(String(entry), entries);
	}
}

#ifdef ___INANITY_SCRIPTING

void FileSystem::Script_loadFile(ScriptCall& call)
{
	call.EnsureArgumentsCount(1);
	call.Return(LoadFile(call.GetString(1)));
}

void FileSystem::Script_saveFile(ScriptCall& call)
{
	call.EnsureArgumentsCount(2);
	SaveFile(call.GetPointer<File>(1), call.GetString(2));
}

SCRIPTABLE_MAP_BEGIN(FileSystem);
	SCRIPTABLE_METHOD(FileSystem, loadFile);
	SCRIPTABLE_METHOD(FileSystem, saveFile);
SCRIPTABLE_MAP_END();

#endif
