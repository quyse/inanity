#include "BlobFileSystem.hpp"
#include "BufferedInputStream.hpp"
#include "../MemoryFile.hpp"
#include "../StreamReader.hpp"
#include "../Exception.hpp"
#include <string.h>

BEGIN_INANITY_DATA

const char BlobFileSystem::Terminator::magicValue[4] = { 'B', 'L', 'O', 'B' };

BlobFileSystem::BlobFileSystem(ptr<Storage> storage)
: storage(storage)
{
	BEGIN_TRY();

	bigsize_t storageSize = storage->GetBigSize();
	bigsize_t size = storageSize;

	//получить терминатор
	if(size < sizeof(Terminator))
		THROW("Can't read terminator");
	Terminator terminator;
	storage->Read(storageSize - sizeof(terminator), sizeof(terminator), &terminator);
	size -= sizeof(terminator);

	//проверить сигнатуру
	if(memcmp(terminator.magic, Terminator::magicValue, sizeof(terminator.magic)) != 0)
		THROW("Invalid magic");

	//получить размер заголовка
	size_t headerSize = 0;
	for(size_t i = 0; i < 4; ++i)
		headerSize += size_t(terminator.headerSize[i]) << (i * 8);

	//проверить, что заголовок читается
	if(size < headerSize)
		THROW("Can't read header");

	//получить читатель заголовка
	ptr<StreamReader> headerReader = NEW(StreamReader(NEW(BufferedInputStream(storage->GetInputStream(size - headerSize, headerSize)))));
	size -= headerSize;

	//считывать файлы, пока есть
	for(;;)
	{
		//считать имя файла
		String fileName = headerReader->ReadString();
		//если оно пустое, значит, это конец
		if(fileName.empty())
			break;

		//считать смещение до файла и его размер
		bigsize_t fileOffset = headerReader->ReadShortlyBig();
		size_t fileSize = headerReader->ReadShortly();

		//проверить, что файл читается
		if(fileOffset > size || fileOffset + fileSize > size)
			THROW("Can't read file " + fileName);

		//добавить файл в файловую систему
		files.insert({ fileName, { fileOffset, fileSize }});
	}

	END_TRY("Can't unpack blob file system");
}

ptr<File> BlobFileSystem::TryLoadFile(const String& fileName)
{
	Files::const_iterator i = files.find(fileName);
	if(i == files.end()) return nullptr;
	ptr<File> file = NEW(MemoryFile(i->second.second));
	storage->Read(i->second.first, i->second.second, file->GetData());
	return file;
}

END_INANITY_DATA
