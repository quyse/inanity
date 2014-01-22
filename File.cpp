#include "File.hpp"
#include "PartFile.hpp"

BEGIN_INANITY

ptr<File> File::Slice(size_t offset, size_t size)
{
	return NEW(PartFile(this, offset, size));
}

END_INANITY
