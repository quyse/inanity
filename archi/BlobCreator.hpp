#ifndef ___INANITY_ARCHI_BLOB_CREATOR_HPP___
#define ___INANITY_ARCHI_BLOB_CREATOR_HPP___

#include "Processor.hpp"
#include <string>

class BlobCreator : public Processor
{
private:
	ptr<FileSystem> fileSystem;
	ptr<Data::BlobFileSystemBuilder> builder;

	void AddDirectory(const String& rootDirectory, const String& namePrefix = "");

public:
	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
