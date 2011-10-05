#include "OutputStream.hpp"
#include "InputStream.hpp"
#include "MemoryFile.hpp"
#include <string.h>

void OutputStream::Write(const void* data, size_t size)
{
	ptr<MemoryFile> file = NEW(MemoryFile(size));
	memcpy(file->GetData(), data, size);
	Write(file);
}

void OutputStream::Write(ptr<File> file)
{
	Write(file->GetData(), file->GetSize());
}

void OutputStream::Flush()
{
}

size_t OutputStream::ReadAllFromStream(ptr<InputStream> inputStream)
{
	static char buffer[0x100000];
	size_t totalRead = 0;
	while(size_t read = inputStream->Read(buffer, sizeof(buffer)))
	{
		Write(buffer, read);
		totalRead += read;
	}
	return totalRead;
}
