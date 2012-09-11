#include "OutputStream.hpp"
#include "InputStream.hpp"
#include "MemoryFile.hpp"
#include "Future.hpp"
#include "scripting_impl.hpp"
#include <string.h>

SCRIPTABLE_MAP_BEGIN(OutputStream, Inanity.OutputStream);
	SCRIPTABLE_METHOD(OutputStream, Flush);
SCRIPTABLE_MAP_END();

ptr<Future<int> > OutputStream::WriteAsync(ptr<File> file)
{
	ptr<Future<int> > future = NEW(Future<int>());
	try
	{
		Write(file->GetData(), file->GetSize());
		future->Result(0);
	}
	catch(Exception* exception)
	{
		future->Error(exception);
	}
	return future;
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
