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

	alSourcei(source, AL_BUFFER, buffer->GetBuffer());
	AlSystem::CheckErrors("Can't set source buffer");

	END_TRY("Can't create OpenAL buffered player");
}

void AlBufferedPlayer::Play(int repeat)
{
	// FIXME: do repeat support
	alSourcePlay(source);
	AlSystem::CheckErrors("Can't play OpenAL buffered player");
}

void AlBufferedPlayer::Pause()
{
	alSourcePause(source);
	AlSystem::CheckErrors("Can't pause OpenAL buffered player");
}

void AlBufferedPlayer::Stop()
{
	alSourceStop(source);
	AlSystem::CheckErrors("Can't stop OpenAL buffered player");
}

bool AlBufferedPlayer::IsPlaying() const
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	AlSystem::CheckErrors("Can't get OpenAL buffered player playing state");
	return state == AL_PLAYING;
}

END_INANITY_AUDIO
