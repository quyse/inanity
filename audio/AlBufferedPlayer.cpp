#include "AlBufferedPlayer.hpp"
#include "AlBuffer.hpp"
#include "AlDevice.hpp"
#include "AlSystem.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

AlBufferedPlayer::AlBufferedPlayer(ptr<AlBuffer> buffer)
: AlPlayer(buffer->GetDevice()), buffer(buffer)
{
	BEGIN_TRY();

	alSourcei(sourceName, AL_BUFFER, buffer->GetName());
	AlSystem::CheckErrors("Can't set source buffer");

	END_TRY("Can't create OpenAL buffered player");
}

AlBufferedPlayer::~AlBufferedPlayer()
{
	// we need to delete source before buffer, so don't rely on ~AlPlayer()
	if(sourceName)
	{
		alDeleteSources(1, &sourceName);
		sourceName = 0;
		AlSystem::CheckErrors("Can't delete source");
	}
}

void AlBufferedPlayer::Play(bool looped)
{
	alSourcei(sourceName, AL_LOOPING, looped ? AL_TRUE : AL_FALSE);
	alSourcePlay(sourceName);
	AlSystem::CheckErrors("Can't play OpenAL buffered player");
}

void AlBufferedPlayer::Pause()
{
	alSourcePause(sourceName);
	AlSystem::CheckErrors("Can't pause OpenAL buffered player");
}

void AlBufferedPlayer::Stop()
{
	alSourceStop(sourceName);
	AlSystem::CheckErrors("Can't stop OpenAL buffered player");
}

bool AlBufferedPlayer::IsPlaying() const
{
	ALint state;
	alGetSourcei(sourceName, AL_SOURCE_STATE, &state);
	AlSystem::CheckErrors("Can't get OpenAL buffered player playing state");
	return state == AL_PLAYING;
}

END_INANITY_AUDIO
