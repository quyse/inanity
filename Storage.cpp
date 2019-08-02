#include "Storage.hpp"
#include "MemoryFile.hpp"
#include "FileInputStream.hpp"
#include "Exception.hpp"

BEGIN_INANITY

void Storage::Read(bigsize_t offset, size_t size, void* data)
{
	if(GetInputStream(offset, size)->Read(data, size) != size)
		THROW("Can't read from storage");
}

ptr<InputStream> Storage::GetInputStream(bigsize_t offset, bigsize_t size)
{
	ptr<MemoryFile> file = NEW(MemoryFile((size_t)size));
	Read(offset, (size_t)size, file->GetData());
	return NEW(FileInputStream(file));
}

END_INANITY
