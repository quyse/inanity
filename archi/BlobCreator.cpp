#include "BlobCreator.hpp"
#include <iostream>

String BlobCreator::GetCommand() const
{
	return "blob";
}

void BlobCreator::PrintHelp() const
{
	std::cout << "Creates a blob from specified directory(-ies). Usage:\n";
	std::cout << GetCommand() << L" <output-blob-file> <directory> [<directory>]...\n";
}

void BlobCreator::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 2)
		THROW("Must be at least 2 arguments for command");

	fileSystem = Platform::FileSystem::GetNativeFileSystem();
	builder = NEW(BlobFileSystemBuilder(fileSystem->SaveStream(arguments[0])));

	for(size_t i = 1; i < arguments.size(); ++i)
		AddDirectory(arguments[i]);

	builder->Finalize();
}

void BlobCreator::AddDirectory(const String& directory, const String& namePrefix)
{
	std::vector<String> entries;
	fileSystem->GetAllDirectoryEntries(directory, entries);
	for(size_t i = 0; i < entries.size(); ++i)
		builder->AddFileStream(entries[i], fileSystem->LoadStream(entries[i]));
}
