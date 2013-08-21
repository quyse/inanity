#include "AlPlayer.hpp"
#include "AlDevice.hpp"
#include "AlSystem.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

AlPlayer::AlPlayer(ptr<AlDevice> device)
: device(device)
{
	BEGIN_TRY();

	alGenSources(1, &source);
	AlSystem::CheckErrors("Can't gen source");

	END_TRY("Can't create OpenAL player");
}

AlPlayer::~AlPlayer()
{
	if(source)
		alDeleteSources(1, &source);
}

END_INANITY_AUDIO
