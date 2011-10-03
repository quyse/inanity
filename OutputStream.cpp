#include "OutputStream.hpp"
#include "InputStream.hpp"

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
