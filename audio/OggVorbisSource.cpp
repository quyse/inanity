#include "OggVorbisSource.hpp"
#include "OggVorbisStream.hpp"
#include "../Storage.hpp"

BEGIN_INANITY_AUDIO

OggVorbisSource::OggVorbisSource(ptr<Storage> storage) : storage(storage)
{
	// get info from temporary stream
	ptr<OggVorbisStream> stream = NEW(OggVorbisStream(storage));
	format = stream->GetFormat();
	samplesCount = stream->GetSamplesCount();
}

Format OggVorbisSource::GetFormat() const
{
	return format;
}

size_t OggVorbisSource::GetSamplesCount() const
{
	return samplesCount;
}

ptr<InputStream> OggVorbisSource::CreateStream()
{
	return NEW(OggVorbisStream(storage));
}

END_INANITY_AUDIO
