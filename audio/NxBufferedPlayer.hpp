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
	bool looped;

	void AppendBuffer();

public:
	NxBufferedPlayer(ptr<NxBufferedSound> sound, const nn::audio::WaveBuffer& buffer, const Format& format);

	// NxPlayer's methods.
	void Tick() override;

	// Player's methods.
	void Play(bool looped) override;
	void Pause() override;
	void Stop() override;
};

END_INANITY_AUDIO

#endif
