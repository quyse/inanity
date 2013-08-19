#ifndef ___INANITY_AUDIO_AL_BUFFERED_PLAYER_HPP___
#define ___INANITY_AUDIO_AL_BUFFERED_PLAYER_HPP___

#include "AlPlayer.hpp"

BEGIN_INANITY_AUDIO

class AlBufferedSound;

/// Class of OpenAL buffered player.
class AlBufferedPlayer : public AlPlayer
{
private:
	ptr<AlBufferedSound> sound;

public:
	AlBufferedPlayer(ptr<AlBufferedSound> sound, ALuint source);

	//** Player3D's methods.
	void Play(int repeat = 1);
	void Pause();
	void Stop();
	bool IsPlaying() const;
	void SetPosition(const Math::vec3& position);
	void SetDirection(const Math::vec3& direction);
	void SetVelocity(const Math::vec3& velocity);
};

END_INANITY_AUDIO

#endif
