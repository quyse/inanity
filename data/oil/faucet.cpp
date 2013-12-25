#include "KeyValueStore.hpp"
#include "../../data/BufferedOutputStream.hpp"
#include "../../data/Lz4CompressStream.hpp"
#include "../../platform/FileSystem.hpp"
#include "../../crypto/GenericHashAlgorithm.hpp"
#include "../../crypto/WhirlpoolStream.hpp"
#include "../../Strings.hpp"
#include "../../File.hpp"
#include "../../Exception.hpp"
#include <iostream>
using namespace Inanity;
using namespace Inanity::Data;
using namespace Inanity::Data::Oil;

void Run(const char* fileName, const char* outputStreamFileName)
{
	ptr<FileSystem> fileSystem = Platform::FileSystem::GetNativeFileSystem();
	ptr<OutputStream> diskOutputStream = fileSystem->SaveStream(outputStreamFileName);
	ptr<Data::Lz4CompressStream> outputStream = NEW(Data::Lz4CompressStream(NEW(BufferedOutputStream(diskOutputStream))));

	ptr<Crypto::HashAlgorithm> hashAlgorithm = NEW(Crypto::GenericHashAlgorithm<Crypto::WhirlpoolStream>());

	ptr<KeyValueStore> store = NEW(KeyValueStore(fileName, outputStream, hashAlgorithm));

	for(;;)
	{
		try
		{
			std::cout << "> ";
			String command;
			std::cin >> command;
			if(command == "exit")
				break;
			else if(command == "get")
			{
				String key;
				std::cin >> key;
				ptr<File> value = store->TryGet(Strings::String2File(key));
				if(value)
					std::cout << Strings::File2String(value) << '\n';
				else
					std::cout << "<no such key>\n";
			}
			else if(command == "set")
			{
				String key, value;
				std::cin >> key >> value;
				store->Set(Strings::String2File(key), Strings::String2File(value));
				store->Checkpoint();
			}
			else if(command == "import_folder")
			{
				String folder;
				std::cin >> folder;
				if(folder.length() && folder[folder.length() - 1] != '/')
					folder += '/';
				std::vector<String> entries;
				fileSystem->GetAllDirectoryEntries(folder, entries);
				for(size_t i = 0; i < entries.size(); ++i)
					if(entries[i].length() && entries[i][entries[i].length() - 1] != '/')
						store->Set(Strings::String2File(entries[i]), fileSystem->LoadFile(entries[i]));
				store->Checkpoint();
			}
			else
				THROW("Invalid command: " + command);
		}
		catch(Exception* exception)
		{
			MakePointer(exception)->PrintStack(std::cout);
			std::cout << '\n';
		}
	}

	outputStream->Flush();
}

int main(int argc, char** argv)
{
	argc--;
	argv++;

	if(argc < 2)
	{
		std::cout << "Inanity Oil Faucet usage:\n"
			"oilfaucet <db file name> <out stream file name>\n";
		return 0;
	}

	try
	{
		Run(argv[0], argv[1]);
	}
	catch(Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
