#ifndef ___INANITY_AUDIO_OGG_VORBIS_SOURCE_HPP___
#define ___INANITY_AUDIO_OGG_VORBIS_SOURCE_HPP___

#include "Source.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

/// Класс источника звуковых данных Ogg Vorbis.
class OggVorbisSource : public Source
{
private:
	Format format;

public:
	OggVorbisSource(ptr<File> file);

	// методы Source
	Format GetFormat() const;
	ptr<InputStream> CreateStream();
};

END_INANITY_AUDIO

#endif
