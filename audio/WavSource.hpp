#ifndef ___INANITY_AUDIO_WAV_SOURCE_HPP___
#define ___INANITY_AUDIO_WAV_SOURCE_HPP___

#include "Source.hpp"

BEGIN_INANITY_AUDIO

/// WAV file sound source.
class WavSource : public Source
{
private:
	ptr<File> dataFile;
	Format format;
	size_t samplesCount;

	struct Header;

public:
	WavSource(ptr<File> file);

	// Source methods
	Format GetFormat() const override;
	size_t GetSamplesCount() const override;
	ptr<File> GetData() override;
};

END_INANITY_AUDIO

#endif
