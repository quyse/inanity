#include "OutputStream.hpp"
#include "InputStream.hpp"
#include "MemoryFile.hpp"
#include "scripting_impl.hpp"
#include <string.h>

SCRIPTABLE_MAP_BEGIN(OutputStream, Inanity.OutputStream);
	SCRIPTABLE_METHOD(OutputStream, WriteFile);
	SCRIPTABLE_METHOD(OutputStream, Flush);
SCRIPTABLE_MAP_END();

void OutputStream::Write(const void* data, size_t size)
{
	ptr<MemoryFile> file = NEW(MemoryFile(size));
	memcpy(file->GetData(), data, size);
	WriteFile(file);
}

void OutputStream::WriteFile(ptr<File> file)
{
	Write(file->GetData(), file->GetSize());
}

void OutputStream::Flush()
{
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
