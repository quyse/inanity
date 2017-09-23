#ifndef ___INANITY_AUDIO_AL_PLAYER_HPP___
#define ___INANITY_AUDIO_AL_PLAYER_HPP___

#include "Player3D.hpp"
#include "al.hpp"

BEGIN_INANITY_AUDIO

class AlDevice;

/// Base OpenAL player class.
class AlPlayer : public Player3D
{
protected:
	ptr<AlDevice> device;
	ALuint sourceName;

public:
	AlPlayer(ptr<AlDevice> device);
	~AlPlayer();

	//** Player's methods.
	void SetVolume(float volume);

	//** Player3D's methods
	void SetPosition(const Math::vec3& position);
	void SetDirection(const Math::vec3& direction);
	void SetVelocity(const Math::vec3& velocity);
};

END_INANITY_AUDIO

#endif
