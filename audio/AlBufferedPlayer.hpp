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
