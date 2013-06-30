#include "OutputStream.hpp"
#include "InputStream.hpp"
#include "MemoryFile.hpp"
#include "meta/impl.hpp"
#include <string.h>

BEGIN_INANITY

META_CLASS(OutputStream, Inanity.OutputStream);
META_CLASS_END();

void OutputStream::Write(const void* data, size_t size)
{
	WriteFile(MemoryFile::CreateViaCopy(data, size));
}

void OutputStream::WriteFile(ptr<File> file)
{
	Write(file->GetData(), file->GetSize());
}

bigsize_t OutputStream::ReadAllFromStream(ptr<InputStream> inputStream)
{
	const size_t bufferSize = 0x10000;
	MemoryFile bufferFile(bufferSize);
	void* buffer = bufferFile.GetData();
	bigsize_t totalRead = 0;
	while(size_t read = inputStream->Read(buffer, bufferSize))
	{
		Write(buffer, read);
		totalRead += read;
	}
	return totalRead;
}

END_INANITY
