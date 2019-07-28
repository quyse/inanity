#include "SubFileSystem.hpp"
#include "File.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "Exception.hpp"

BEGIN_INANITY

SubFileSystem::SubFileSystem(ptr<FileSystem> fileSystem, const String& userPath)
: fileSystem(fileSystem), path(userPath)
{
	if(path.length() && path[path.length() - 1] != '/')
		path += "/";
}

String SubFileSystem::GetName(const String& fileName) const
{
	if(fileName.empty() || fileName[0] != '/')
		THROW("File name should begin with slash");
	return path + fileName.substr(1);
}

ptr<File> SubFileSystem::LoadFile(const String& fileName)
{
	return fileSystem->LoadFile(GetName(fileName));
}

ptr<File> SubFileSystem::TryLoadFile(const String& fileName)
{
	return fileSystem->TryLoadFile(GetName(fileName));
}

ptr<InputStream> SubFileSystem::LoadStream(const String& fileName)
{
	return fileSystem->LoadStream(GetName(fileName));
}

void SubFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	fileSystem->SaveFile(file, GetName(fileName));
}

ptr<OutputStream> SubFileSystem::SaveStream(const String& fileName)
{
	return fileSystem->SaveStream(GetName(fileName));
}

time_t SubFileSystem::GetFileMTime(const String& fileName)
{
	return fileSystem->GetFileMTime(GetName(fileName));
}

void SubFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	fileSystem->GetDirectoryEntries(GetName(directoryName), entries);
}

void SubFileSystem::GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	fileSystem->GetAllDirectoryEntries(GetName(directoryName), entries);
}

void SubFileSystem::MakeDirectory(const String& directoryName)
{
	fileSystem->MakeDirectory(GetName(directoryName));
}

void SubFileSystem::DeleteEntry(const String& entryName)
{
	fileSystem->DeleteEntry(GetName(entryName));
}

FileSystem::EntryType SubFileSystem::GetEntryType(const String& entryName) const
{
	return fileSystem->GetEntryType(GetName(entryName));
}

ptr<FileSystem> SubFileSystem::GetSubFileSystem(const String& folderName)
{
	return NEW(SubFileSystem(fileSystem, GetName(folderName)));
}

END_INANITY
