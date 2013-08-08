#include "BlobFileSystemBuilder.hpp"
#include "BlobFileSystem.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "MemoryStream.hpp"
#include "StreamWriter.hpp"
#include "FileInputStream.hpp"
#include "File.hpp"
#include "Exception.hpp"
#include "meta/impl.hpp"
#include <cstring>

BEGIN_INANITY

META_CLASS(BlobFileSystemBuilder, Inanity.BlobFileSystemBuilder);
	META_CONSTRUCTOR(ptr<OutputStream>);
	META_METHOD(AddFile);
	META_METHOD(AddFileStream);
	META_METHOD(Finalize);
META_CLASS_END();

BlobFileSystemBuilder::BlobFileSystemBuilder(ptr<OutputStream> outputStream)
{
	try
	{
		outputWriter = NEW(StreamWriter(outputStream));

		headerStream = NEW(MemoryStream());
		headerWriter = NEW(StreamWriter(headerStream));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create blob file system builder", exception);
	}
}

void BlobFileSystemBuilder::AddFile(const String& fileName, ptr<File> file, size_t alignment)
{
	AddFileStream(fileName, NEW(FileInputStream(file)), alignment);
}

void BlobFileSystemBuilder::AddFileStream(const String& fileName, ptr<InputStream> fileStream, size_t alignment)
{
	try
	{
		//сделать выравнивание
		outputWriter->WriteGap(alignment);
		//запомнить текущую позицию
		size_t fileOffset = outputWriter->GetWrittenSize();
		//записать данные файла
		char buffer[0x10000];
		size_t fileSize = 0;
		for(size_t length; (length = fileStream->Read(buffer, sizeof(buffer))); fileSize += length)
			outputWriter->Write(buffer, length);

		//добавить запись о файле
		headerWriter->WriteString(fileName);
		headerWriter->WriteShortly(fileOffset);
		headerWriter->WriteShortly(fileSize);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't add file stream", exception);
	}
}

void BlobFileSystemBuilder::Finalize()
{
	try
	{
		//добавить заглушку в заголовок
		headerWriter->WriteString(String());
		//завершить формирование заголовка
		headerWriter = 0;
		ptr<File> headerFile = headerStream->ToFile();
		//записать заголовок
		outputWriter->Write(headerFile->GetData(), headerFile->GetSize());
		//записать терминатор
		BlobFileSystem::Terminator terminator;
		memcpy(terminator.magic, BlobFileSystem::Terminator::magicValue, sizeof(terminator.magic));
		terminator.headerSize = headerFile->GetSize();
		outputWriter->Write(terminator);

		//всё!
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't finalize blob file system", exception);
	}
}

END_INANITY
