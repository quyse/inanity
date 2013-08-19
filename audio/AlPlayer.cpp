#include "AlPlayer.hpp"
#include "AlDevice.hpp"

BEGIN_INANITY_AUDIO

AlPlayer::AlPlayer(ptr<AlDevice> device, ALuint source)
: device(device), source(source) {}

AlPlayer::~AlPlayer()
{
	if(source)
		alDeleteSources(1, &source);
}

END_INANITY_AUDIO
