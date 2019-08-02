#include "File.hpp"
#include "MemoryFile.hpp"
#include "PartFile.hpp"
#include "FileInputStream.hpp"
#include "Exception.hpp"
#include <string.h>

BEGIN_INANITY

bool File::Comparer::operator()(File* a, File* b) const
{
	size_t asize = a->GetSize();
	size_t bsize = b->GetSize();
	int r = memcmp(a->GetData(), b->GetData(), asize < bsize ? asize : bsize);
	return (r == 0) ? (asize < bsize) : (r < 0);
}

ptr<File> File::Slice(size_t offset, size_t size)
{
	return NEW(PartFile(this, offset, size));
}

ptr<File> File::SliceFrom(size_t offset)
{
	return Slice(offset, GetSize() - offset);
}

ptr<File> File::Concat(ptr<File> other)
{
	size_t size = GetSize();
	size_t otherSize = other->GetSize();
	ptr<MemoryFile> resultFile = NEW(MemoryFile(size + otherSize));
	char* resultData = (char*)resultFile->GetData();
	memcpy(resultData, GetData(), size);
	memcpy(resultData + size, other->GetData(), otherSize);
	return resultFile;
}

bigsize_t File::GetBigSize() const
{
	return GetSize();
}

void File::Read(bigsize_t offset, size_t size, void* data)
{
	memcpy(data, (const uint8_t*)GetData() + offset, size);
}

ptr<InputStream> File::GetInputStream(bigsize_t offset, bigsize_t size)
{
	return NEW(FileInputStream(Slice((size_t)offset, (size_t)size)));
}

END_INANITY
