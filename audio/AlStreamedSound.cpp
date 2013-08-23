#include "AlStreamedSound.hpp"
#include "AlDevice.hpp"
#include "Source.hpp"
#include "AlStreamedPlayer.hpp"

BEGIN_INANITY_AUDIO

AlStreamedSound::AlStreamedSound(ptr<AlDevice> device, ptr<Source> source)
: device(device), source(source) {}

ptr<Player> AlStreamedSound::CreatePlayer()
{
	return CreatePlayer3D();
}

ptr<Player3D> AlStreamedSound::CreatePlayer3D()
{
	return NEW(AlStreamedPlayer(device, source));
}

END_INANITY_AUDIO
