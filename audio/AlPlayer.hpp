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
	ALuint source;

public:
	AlPlayer(ptr<AlDevice> device, ALuint source);
	~AlPlayer();
};

END_INANITY_AUDIO

#endif
