#include "AlBufferedSound.hpp"
#include "AlDevice.hpp"
#include "AlBufferedPlayer.hpp"

BEGIN_INANITY_AUDIO

AlBufferedSound::AlBufferedSound(ptr<AlDevice> device, ALuint buffer)
: device(device), buffer(buffer) {}

AlBufferedSound::~AlBufferedSound()
{
	if(buffer)
		alDeleteBuffers(1, &buffer);
}

ptr<AlDevice> AlBufferedSound::GetDevice() const
{
	return device;
}

ALuint AlBufferedSound::GetBuffer() const
{
	return buffer;
}

ptr<Player> AlBufferedSound::CreatePlayer()
{
	return CreatePlayer3D();
}

ptr<Player3D> AlBufferedSound::CreatePlayer3D()
{
	return NEW(AlBufferedPlayer(this));
}

END_INANITY_AUDIO
