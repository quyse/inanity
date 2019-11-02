#ifndef ___INANITY_AUDIO_NX_BUFFERED_PLAYER_HPP___
#define ___INANITY_AUDIO_NX_BUFFERED_PLAYER_HPP___

#include "NxPlayer.hpp"

BEGIN_INANITY_AUDIO

class NxBufferedSound;

class NxBufferedPlayer : public NxPlayer
{
private:
	ptr<NxBufferedSound> sound;
	const nn::audio::WaveBuffer& buffer;
	bool bufferAdded;

	//*** NxPlayer's methods.
	bool RequestWaveBuffer(const nn::audio::WaveBuffer*) override;
	void Reset() override;

public:
	NxBufferedPlayer(ptr<NxBufferedSound> sound, const nn::audio::WaveBuffer& buffer, const Format& format);
};

END_INANITY_AUDIO

#endif
