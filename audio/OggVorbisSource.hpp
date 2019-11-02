#ifndef ___INANITY_AUDIO_OGG_VORBIS_SOURCE_HPP___
#define ___INANITY_AUDIO_OGG_VORBIS_SOURCE_HPP___

#include "Source.hpp"

BEGIN_INANITY

class Storage;

END_INANITY

BEGIN_INANITY_AUDIO

/// Ogg Vorbis source.
class OggVorbisSource : public Source
{
private:
	ptr<Storage> storage;
	Format format;
	size_t samplesCount;

public:
	OggVorbisSource(ptr<Storage> storage);

	// Source's methods.
	Format GetFormat() const;
	size_t GetSamplesCount() const;
	ptr<InputStream> CreateStream();
};

END_INANITY_AUDIO

#endif
