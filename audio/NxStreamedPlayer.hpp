#ifndef ___INANITY_AUDIO_NX_STREAMED_PLAYER_HPP___
#define ___INANITY_AUDIO_NX_STREAMED_PLAYER_HPP___

#include "NxPlayer.hpp"

BEGIN_INANITY

class File;
class InputStream;

END_INANITY

BEGIN_INANITY_AUDIO

class Source;

class NxStreamedPlayer : public NxPlayer
{
private:
	ptr<Source> source;
	ptr<InputStream> stream;

	/// Length of sound in one buffer, in ms.
	static const size_t bufferTime = 500;
	/// Number of buffers.
	static const int buffersCount = 3;

	size_t sampleSize;
	nn::audio::WaveBuffer buffers[buffersCount];
	ptr<File> bufferFiles[buffersCount];

	//*** NxPlayer's methods.
	bool RequestWaveBuffer(const nn::audio::WaveBuffer* buffer) override;
	void Reset() override;

public:
	NxStreamedPlayer(ptr<NxSystem> system, ptr<Source> source);
};

END_INANITY_AUDIO

#endif
