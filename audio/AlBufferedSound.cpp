#include "AlBufferedSound.hpp"
#include "AlBuffer.hpp"
#include "AlBufferedPlayer.hpp"

BEGIN_INANITY_AUDIO

AlBufferedSound::AlBufferedSound(ptr<AlBuffer> buffer)
: buffer(buffer) {}

ptr<AlBuffer> AlBufferedSound::GetBuffer() const
{
	return buffer;
}

ptr<Player> AlBufferedSound::CreatePlayer()
{
	return CreatePlayer3D();
}

ptr<Player3D> AlBufferedSound::CreatePlayer3D()
{
	return NEW(AlBufferedPlayer(buffer));
}

END_INANITY_AUDIO
