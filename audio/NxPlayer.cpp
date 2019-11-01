#include "NxPlayer.hpp"
#include "NxSystem.hpp"
#include "Format.hpp"
#include <cstdio>

BEGIN_INANITY_AUDIO

NxPlayer::NxPlayer(ptr<NxSystem> system, const Format& format)
: system(system), channelsCount(format.channelsCount)
{
	system->CreateVoice(voice, format);
	// set default volume and panning (otherwise mix volumes are zero)
	SetVolume(1);
	SetPanning(0);

	system->RegisterPlayer(this);
}

NxPlayer::~NxPlayer()
{
	system->UnregisterPlayer(this);
	system->DeleteVoice(voice);
}

bool NxPlayer::IsPlaying() const
{
	return nn::audio::GetVoicePlayState(&voice) == nn::audio::VoiceType::PlayState_Play;
}

void NxPlayer::SetVolume(float volume)
{
	nn::audio::SetVoiceVolume(&voice, volume);
}

void NxPlayer::SetPanning(float pan)
{
	// set pan only for mono voices
	if(channelsCount != 1) return;

	system->SetVoicePanning(voice, pan);
}

void NxPlayer::SetPitch(float pitch)
{
	nn::audio::SetVoicePitch(&voice, pitch);
}

END_INANITY_AUDIO
