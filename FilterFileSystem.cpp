#include "FilterFileSystem.hpp"
#include "File.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

FilterFileSystem::FilterFileSystem(ptr<FileSystem> fileSystem)
: fileSystem(fileSystem) {}

ptr<File> FilterFileSystem::LoadFile(const String& fileName)
{
	return fileSystem->LoadFile(fileName);
}

ptr<File> FilterFileSystem::TryLoadFile(const String& fileName)
{
	return fileSystem->TryLoadFile(fileName);
}

ptr<InputStream> FilterFileSystem::LoadStream(const String& fileName)
{
	return fileSystem->LoadStream(fileName);
}

void FilterFileSystem::SaveFile(ptr<File> file, const String& fileName)
{
	fileSystem->SaveFile(file, fileName);
}

ptr<OutputStream> FilterFileSystem::SaveStream(const String& fileName)
{
	return fileSystem->SaveStream(fileName);
}

void FilterFileSystem::GetFileNames(std::vector<String>& fileNames) const
{
	fileSystem->GetFileNames(fileNames);
}

void FilterFileSystem::GetDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	fileSystem->GetDirectoryEntries(directoryName, entries);
}

void FilterFileSystem::GetAllDirectoryEntries(const String& directoryName, std::vector<String>& entries) const
{
	fileSystem->GetAllDirectoryEntries(directoryName, entries);
}
