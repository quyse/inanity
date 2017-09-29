#include "AlPlayer.hpp"
#include "AlDevice.hpp"
#include "AlSystem.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

AlPlayer::AlPlayer(ptr<AlDevice> device)
: device(device)
{
	BEGIN_TRY();

	alGenSources(1, &sourceName);
	AlSystem::CheckErrors("Can't gen source");

	END_TRY("Can't create OpenAL player");
}

AlPlayer::~AlPlayer()
{
	if(sourceName)
	{
		alSourceStop(sourceName);
		alDeleteSources(1, &sourceName);
		AlSystem::CheckErrors("Can't delete source");
	}
}

void AlPlayer::SetVolume(float volume)
{
	alSourcef(sourceName, AL_GAIN, volume);
	AlSystem::CheckErrors("Can't set OpenAL player volume");
}

void AlPlayer::SetPitch(float pitch)
{
	alSourcef(sourceName, AL_PITCH, pitch);
	AlSystem::CheckErrors("Can't set OpenAL player pitch");
}

void AlPlayer::SetPosition(const Math::vec3& position)
{
	alSource3f(sourceName, AL_POSITION, position.x, position.y, position.z);
	AlSystem::CheckErrors("Can't set OpenAL player position");
}

void AlPlayer::SetDirection(const Math::vec3& direction)
{
	alSource3f(sourceName, AL_DIRECTION, direction.x, direction.y, direction.z);
	AlSystem::CheckErrors("Can't set OpenAL player direction");
}

void AlPlayer::SetVelocity(const Math::vec3& velocity)
{
	alSource3f(sourceName, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	AlSystem::CheckErrors("Can't set OpenAL player velocity");
}

END_INANITY_AUDIO
