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
	THROW_PRIMARY_EXCEPTION("Getting file names in this filesystem in not supported");
}
