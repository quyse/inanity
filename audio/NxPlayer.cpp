#include "NxPlayer.hpp"
#include "NxSystem.hpp"
#include "Format.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

NxPlayer::NxPlayer(ptr<NxSystem> system, const Format& format)
: system(system), channelsCount(format.channelsCount), appendedBuffersCount(0), playing(false), looping(false), stopped(true)
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

void NxPlayer::QueueWaveBuffer(const nn::audio::WaveBuffer* buffer)
{
	bufferQueue.push(buffer);
}

void NxPlayer::AppendWaveBuffer(const nn::audio::WaveBuffer* buffer)
{
	if(!nn::audio::AppendWaveBuffer(&voice, buffer))
		THROW("Can't append wave buffer");

	++appendedBuffersCount;
}

void NxPlayer::Tick()
{
	// get all released buffers back to buffer queue
	for(;;)
	{
		const nn::audio::WaveBuffer* buffer = nn::audio::GetReleasedWaveBuffer(&voice);
		if(!buffer) break;
		bufferQueue.push(buffer);
		--appendedBuffersCount;
	}

	// append buffers until we can
	while(!stopped && !bufferQueue.empty())
	{
		const nn::audio::WaveBuffer* buffer = bufferQueue.front();

		// request buffer; if appended correctly
		if(RequestWaveBuffer(buffer))
		{
			// great, continue
			bufferQueue.pop();
		}
		// otherwise there's no more data
		else
		{
			Reset();
			if(!looping)
			{
				stopped = true;
			}
		}
	}

	// if there's no more data and no buffers appended to the voice
	if(playing && stopped && appendedBuffersCount == 0)
	{
		nn::audio::SetVoicePlayState(&voice, nn::audio::VoiceType::PlayState_Stop);
		playing = false;
	}
}

void NxPlayer::Play(bool looped)
{
	nn::audio::SetVoicePlayState(&voice, nn::audio::VoiceType::PlayState_Play);
	playing = true;
	looping = looped;
	stopped = false;
}

void NxPlayer::Pause()
{
	nn::audio::SetVoicePlayState(&voice, nn::audio::VoiceType::PlayState_Pause);
	playing = false;
}

void NxPlayer::Stop()
{
	nn::audio::SetVoicePlayState(&voice, nn::audio::VoiceType::PlayState_Stop);
	playing = false;
	looping = false;
	stopped = true;
	Reset();
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
	system->SetVoicePanning(voice, channelsCount, pan);
}

void NxPlayer::SetPitch(float pitch)
{
	nn::audio::SetVoicePitch(&voice, pitch);
}

END_INANITY_AUDIO
