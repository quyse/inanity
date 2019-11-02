#include "NxStreamedSound.hpp"
#include "NxSystem.hpp"
#include "NxStreamedPlayer.hpp"
#include "Source.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

NxStreamedSound::NxStreamedSound(ptr<NxSystem> system, ptr<Source> source)
: system(system), source(source)
{}

ptr<Player> NxStreamedSound::CreatePlayer()
{
	return NEW(NxStreamedPlayer(system, source));
}

ptr<Player3D> NxStreamedSound::CreatePlayer3D()
{
	THROW("Player3D is not supported");
}

END_INANITY_AUDIO
