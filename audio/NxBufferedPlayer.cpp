#include "NxBufferedPlayer.hpp"
#include "NxBufferedSound.hpp"
#include "NxSystem.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

NxBufferedPlayer::NxBufferedPlayer(ptr<NxBufferedSound> sound, const nn::audio::WaveBuffer& buffer, const Format& format)
: NxPlayer(sound->GetSystem(), format), sound(sound), buffer(buffer), looped(false)
{
	AppendBuffer();
}

void NxBufferedPlayer::AppendBuffer()
{
	if(!nn::audio::AppendWaveBuffer(&voice, &buffer))
		THROW("Can't append wave buffer");
}

void NxBufferedPlayer::Tick()
{
	const nn::audio::WaveBuffer* releasedBuffer = nn::audio::GetReleasedWaveBuffer(&voice);
	if(releasedBuffer && releasedBuffer == &buffer)
	{
		AppendBuffer();
		nn::audio::SetVoicePlayState(&voice, looped ? nn::audio::VoiceType::PlayState_Play : nn::audio::VoiceType::PlayState_Stop);
	}
}

void NxBufferedPlayer::Play(bool looped)
{
	this->looped = looped;
	nn::audio::SetVoicePlayState(&voice, nn::audio::VoiceType::PlayState_Play);
}

void NxBufferedPlayer::Pause()
{
	nn::audio::SetVoicePlayState(&voice, nn::audio::VoiceType::PlayState_Pause);
}

void NxBufferedPlayer::Stop()
{
	nn::audio::SetVoicePlayState(&voice, nn::audio::VoiceType::PlayState_Stop);
}

END_INANITY_AUDIO
