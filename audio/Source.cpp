#include "Source.hpp"

BEGIN_INANITY_AUDIO

size_t Source::GetSize() const
{
	Format format = GetFormat();
	return GetSamplesCount() * format.bitsPerSample * format.channelsCount / 8;
}

END_INANITY_AUDIO
