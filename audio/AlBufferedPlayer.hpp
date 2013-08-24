#ifndef ___INANITY_AUDIO_AL_BUFFERED_PLAYER_HPP___
#define ___INANITY_AUDIO_AL_BUFFERED_PLAYER_HPP___

#include "AlPlayer.hpp"

BEGIN_INANITY_AUDIO

class AlBuffer;

/// Class of OpenAL buffered player.
class AlBufferedPlayer : public AlPlayer
{
private:
	ptr<AlBuffer> buffer;

public:
	AlBufferedPlayer(ptr<AlBuffer> buffer);

	//** Player's methods.
	void Play(bool looped = false);
	void Pause();
	void Stop();
	bool IsPlaying() const;
};

END_INANITY_AUDIO

#endif
