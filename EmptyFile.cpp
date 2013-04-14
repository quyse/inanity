#include "EmptyFile.hpp"

BEGIN_INANITY

void* EmptyFile::GetData() const
{
	return 0;
}

size_t EmptyFile::GetSize() const
{
	return 0;
}

END_INANITY
