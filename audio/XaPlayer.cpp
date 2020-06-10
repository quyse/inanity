#include "XaPlayer.hpp"
#include "XaSystem.hpp"
#include "XaDevice.hpp"
#include "../File.hpp"
#include "../Exception.hpp"
#include <x3daudio.h>

BEGIN_INANITY_AUDIO

XaPlayer::XaPlayer(ptr<XaDevice> device)
: device(device), voice(nullptr), volume(1.0f), pan(0.0f), pitch(1.0f), channelsCount(0), playing(false), onBufferEndCounter(0)
{}

XaPlayer::~XaPlayer()
{
	SetVoice(nullptr);
}

void XaPlayer::SetVoice(IXAudio2SourceVoice* voice)
{
	if(this->voice)
	{
		StopPlaying();
		this->voice->DestroyVoice();
		channelsCount = 0;
	}
	this->voice = voice;
	if(this->voice)
	{
		{
			XAUDIO2_VOICE_DETAILS details;
			this->voice->GetVoiceDetails(&details);
			channelsCount = details.InputChannels;
		}

		ApplyVolume();
		ApplyPan();
		ApplyPitch();
	}
}

void XaPlayer::PushBuffer(ptr<File> file)
{
	files.push(file);
	XAUDIO2_BUFFER buffer;
	buffer.Flags = 0;
	buffer.AudioBytes = (UINT32)file->GetSize();
	buffer.pAudioData = (const BYTE*)file->GetData();
	buffer.PlayBegin = 0;
	buffer.PlayLength = 0;
	buffer.LoopBegin = 0;
	buffer.LoopLength = 0;
	buffer.LoopCount = 0;
	buffer.pContext = nullptr;
	if(FAILED(voice->SubmitSourceBuffer(&buffer)))
		THROW("Can't push buffer to XAudio2 source voice");
}

void XaPlayer::StartPlaying()
{
	if(voice)
	{
		if(FAILED(voice->Start(0, device->GetSystem()->GetOperationSet())))
			THROW("Can't start XAudio2 source voice");
		playing = true;
		device->GetSystem()->RegisterPlayer(this);
	}
}

void XaPlayer::StopPlaying()
{
	if(voice)
	{
		if(FAILED(voice->Stop(0, device->GetSystem()->GetOperationSet())))
			THROW("Can't stop XAudio2 source voice");
		playing = false;
		device->GetSystem()->UnregisterPlayer(this);
	}
}

void XaPlayer::ApplyVolume()
{
	if(FAILED(voice->SetVolume(volume, device->GetSystem()->GetOperationSet())))
		THROW("Can't apply volume to XAudio2 source voice");
}

void XaPlayer::ApplyPan()
{
	// set pan only for mono voices
	if(channelsCount != 1) return;

	float left = 0.5f - pan * 0.5f;
	float right = 0.5f + pan * 0.5f;

	DWORD channelMask = device->GetChannelMask();
	float outputMatrix[32] = {};
	int channel = 0;
	// see https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/ksmedia/ns-ksmedia-waveformatextensible
	if(channelMask & SPEAKER_FRONT_LEFT)            outputMatrix[channel++] = left;
	if(channelMask & SPEAKER_FRONT_RIGHT)           outputMatrix[channel++] = right;
	if(channelMask & SPEAKER_FRONT_CENTER)          outputMatrix[channel++] = 0;
	if(channelMask & SPEAKER_LOW_FREQUENCY)         outputMatrix[channel++] = 0;
	if(channelMask & SPEAKER_BACK_LEFT)             outputMatrix[channel++] = left;
	if(channelMask & SPEAKER_BACK_RIGHT)            outputMatrix[channel++] = right;
	if(channelMask & SPEAKER_FRONT_LEFT_OF_CENTER)  outputMatrix[channel++] = left;
	if(channelMask & SPEAKER_FRONT_RIGHT_OF_CENTER) outputMatrix[channel++] = right;
	if(channelMask & SPEAKER_BACK_CENTER)           outputMatrix[channel++] = 0;
	if(channelMask & SPEAKER_SIDE_LEFT)             outputMatrix[channel++] = left;
	if(channelMask & SPEAKER_SIDE_RIGHT)            outputMatrix[channel++] = right;
	if(channelMask & SPEAKER_TOP_CENTER)            outputMatrix[channel++] = 0;
	if(channelMask & SPEAKER_TOP_FRONT_LEFT)        outputMatrix[channel++] = left;
	if(channelMask & SPEAKER_TOP_FRONT_CENTER)      outputMatrix[channel++] = 0;
	if(channelMask & SPEAKER_TOP_FRONT_RIGHT)       outputMatrix[channel++] = right;
	if(channelMask & SPEAKER_TOP_BACK_LEFT)         outputMatrix[channel++] = left;
	if(channelMask & SPEAKER_TOP_BACK_CENTER)       outputMatrix[channel++] = 0;
	if(channelMask & SPEAKER_TOP_BACK_RIGHT)        outputMatrix[channel++] = right;

	if(FAILED(voice->SetOutputMatrix(nullptr, 1, channel, outputMatrix, device->GetSystem()->GetOperationSet())))
		THROW("Can't set XAudio2 voice output matrix");
}

void XaPlayer::ApplyPitch()
{
	if(FAILED(voice->SetFrequencyRatio(pitch, device->GetSystem()->GetOperationSet())))
		THROW("Can't apply pitch to XAudio2 source voice");
}

void XaPlayer::Tick()
{
	for(; onBufferEndCounter > 0; --onBufferEndCounter)
		if(!files.empty())
			files.pop();

	if(files.empty())
		playing = false;
}

bool XaPlayer::IsPlaying() const
{
	return playing;
}

void XaPlayer::SetVolume(float volume)
{
	this->volume = volume;
	if(voice)
		ApplyVolume();
}

void XaPlayer::SetPanning(float pan)
{
	this->pan = pan;
	if(voice)
		ApplyPan();
}

void XaPlayer::SetPitch(float pitch)
{
	this->pitch = pitch;
	if(voice)
		ApplyPitch();
}

void XaPlayer::SetPosition(const Math::vec3& position)
{
	// not implemented yet
}

void XaPlayer::SetDirection(const Math::vec3& direction)
{
	// not implemented yet
}

void XaPlayer::SetVelocity(const Math::vec3& velocity)
{
	// not implemented yet
}


void CALLBACK XaPlayer::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{}

void CALLBACK XaPlayer::OnVoiceProcessingPassEnd()
{}

void CALLBACK XaPlayer::OnStreamEnd()
{}

void CALLBACK XaPlayer::OnBufferStart(void* pBufferContext)
{}

void CALLBACK XaPlayer::OnBufferEnd(void* pBufferContext)
{
	++onBufferEndCounter;
}

void CALLBACK XaPlayer::OnLoopEnd(void* pBufferContext)
{}

void CALLBACK XaPlayer::OnVoiceError(void* pBufferContext, HRESULT Error)
{}

END_INANITY_AUDIO
