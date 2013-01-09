#include "Source.hpp"

size_t Source::GetSize() const
{
	Format format = GetFormat();
	return GetSamplesCount() * format.bitsPerSample * format.channelsCount / 8;
}
