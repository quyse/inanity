#include "AlBufferedSound.hpp"
#include "AlDevice.hpp"
#include "AlBufferedPlayer.hpp"
#include "AlSystem.hpp"
#include "../Exception.hpp"

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

ptr<Player> AlBufferedSound::CreatePlayer()
{
	return CreatePlayer3D();
}

ptr<Player3D> AlBufferedSound::CreatePlayer3D()
{
	BEGIN_TRY();

	ALuint source;
	alGenSources(1, &source);
	AlSystem::CheckErrors("Can't gen source");

	alSourcei(source, AL_BUFFER, buffer);
	AlSystem::CheckErrors("Can't set source buffer");

	return NEW(AlBufferedPlayer(this, source));

	END_TRY("Can't create OpenAL player");
}

END_INANITY_AUDIO
