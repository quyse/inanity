#include "BlobFileSystem.hpp"
#include "TempFileSystem.hpp"
#include "../PartFile.hpp"
#include "../FileInputStream.hpp"
#include "../StreamReader.hpp"
#include "../Exception.hpp"
#include <string.h>

BEGIN_INANITY_DATA

const char BlobFileSystem::Terminator::magicValue[4] = { 'B', 'L', 'O', 'B' };

void BlobFileSystem::Unpack(ptr<File> file, ptr<FileSystem> fileSystem)
{
	BEGIN_TRY();

	void* fileData = file->GetData();
	size_t fileSize = file->GetSize();
	size_t size = fileSize;

	//получить терминатор
	if(size < sizeof(Terminator))
		THROW("Can't read terminator");
	Terminator* terminator = (Terminator*)((char*)fileData + fileSize) - 1;
	size -= sizeof(*terminator);

	//проверить сигнатуру
	if(memcmp(terminator->magic, Terminator::magicValue, sizeof(terminator->magic)) != 0)
		THROW("Invalid magic");

	//проверить, что заголовок читается
	if(size < terminator->headerSize)
		THROW("Can't read header");

	//получить читатель заголовка
	ptr<StreamReader> headerReader = NEW(StreamReader(NEW(FileInputStream(
		NEW(PartFile(file, (char*)terminator - terminator->headerSize, terminator->headerSize))))));
	size -= terminator->headerSize;

	//считывать файлы, пока есть
	for(;;)
	{
		//считать имя файла
		String fileName = headerReader->ReadString();
		//если оно пустое, значит, это конец
		if(fileName.empty())
			break;

		//считать смещение до файла и его размер
		size_t fileOffset = headerReader->ReadShortly();
		size_t fileSize = headerReader->ReadShortly();

		//проверить, что файл читается
		if(fileOffset > size || size - fileOffset < fileSize)
			THROW("Can't read file " + fileName);

		//добавить файл в файловую систему
		fileSystem->SaveFile(NEW(PartFile(file, (char*)fileData + fileOffset, fileSize)), fileName);
	}

	END_TRY("Can't unpack blob file system");
}

ptr<FileSystem> BlobFileSystem::Load(ptr<File> file)
{
	ptr<FileSystem> fileSystem = NEW(TempFileSystem());
	Unpack(file, fileSystem);
	return fileSystem;
}

END_INANITY_DATA
