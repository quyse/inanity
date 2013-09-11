#include "Sound.hpp"
#include "Player.hpp"
#include "Player3D.hpp"

BEGIN_INANITY_AUDIO

ptr<Player> Sound::CreatePlayer()
{
	return CreatePlayer3D();
}

END_INANITY_AUDIO
