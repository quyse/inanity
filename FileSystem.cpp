#include "FileSystem.hpp"
#include "File.hpp"
#include "FileInputStream.hpp"
#include "Exception.hpp"

ptr<File> FileSystem::LoadFile(const std::wstring& fileName)
{
	ptr<File> file = TryLoadFile(fileName);
	if(file)
		return file;
	THROW_PRIMARY_EXCEPTION(L"Can't load file " + fileName);
}

ptr<File> FileSystem::TryLoadFile(const std::wstring& fileName)
{
	try
	{
		return LoadFile(fileName);
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		return nullptr;
	}
}

ptr<InputStream> FileSystem::LoadFileAsStream(const std::wstring& fileName)
{
	try
	{
		return NEW(FileInputStream(LoadFile(fileName)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(L"Can't load file " + fileName + L" as stream", exception);
	}
}

void FileSystem::SaveFile(ptr<File> file, const std::wstring& fileName)
{
	THROW_PRIMARY_EXCEPTION(L"Saving files in this filesystem is not supported");
}

ptr<OutputStream> FileSystem::SaveFileAsStream(const std::wstring& fileName)
{
	THROW_PRIMARY_EXCEPTION(L"Saving files as stream in this filesystem is not supported");
}

void FileSystem::GetFileNames(std::vector<std::wstring>& fileNames) const
{
	THROW_PRIMARY_EXCEPTION(L"Getting file names in this filesystem in not supported");
}
