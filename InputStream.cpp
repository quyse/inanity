#include "InputStream.hpp"
#include "MemoryFile.hpp"
#include "PartFile.hpp"
#include "Exception.hpp"
#include <algorithm>

BEGIN_INANITY

bigsize_t InputStream::Skip(bigsize_t size)
{
	BEGIN_TRY();

	char buffer[0x1000];
	bigsize_t skipped = 0;
	while(size)
	{
		size_t read = Read(buffer, (size_t)std::min<bigsize_t>(size, sizeof(buffer)));
		if(!read)
			break;
		size -= read;
		skipped += read;
	}
	return skipped;

	END_TRY("Can't skip data in input stream");
}

ptr<File> InputStream::Read(size_t size)
{
	BEGIN_TRY();

	// read data
	ptr<File> file = NEW(MemoryFile(size));
	size_t read = Read(file->GetData(), size);

	// if read less data than requested, slice the file
	if(read < size)
		file = file->Slice(0, read);

	return file;

	END_TRY("Can't read data as file from input stream");
}

END_INANITY
