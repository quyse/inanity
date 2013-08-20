#ifndef ___INANITY_AUDIO_OGG_VORBIS_SOURCE_HPP___
#define ___INANITY_AUDIO_OGG_VORBIS_SOURCE_HPP___

#include "Source.hpp"

BEGIN_INANITY_AUDIO

/// Класс источника звуковых данных Ogg Vorbis.
class OggVorbisSource : public Source
{
private:
	ptr<File> file;
	Format format;
	size_t samplesCount;
	ptr<File> dataFile;

public:
	OggVorbisSource(ptr<File> file);

	// методы Source
	Format GetFormat() const;
	size_t GetSamplesCount() const;
	ptr<File> GetData();
	ptr<InputStream> CreateStream();
};

END_INANITY_AUDIO

#endif
