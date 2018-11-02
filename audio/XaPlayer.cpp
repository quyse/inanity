#include "XaPlayer.hpp"
#include "XaDevice.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaPlayer::XaPlayer(ptr<XaDevice> device)
: device(device), voice(nullptr), volume(1.0f), pitch(1.0f), playing(false)
{}

XaPlayer::~XaPlayer()
{
	SetVoice(nullptr);
}

void XaPlayer::SetVoice(IXAudio2SourceVoice* voice)
{
	if(this->voice)
		this->voice->DestroyVoice();
	this->voice = voice;
	if(this->voice)
	{
		ApplyVolume();
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
		if(FAILED(voice->Start()))
			THROW("Can't start XAudio2 source voice");
		playing = true;
	}
}

void XaPlayer::StopPlaying()
{
	if(voice)
	{
		if(FAILED(voice->Stop()))
			THROW("Can't stop XAudio2 source voice");
		playing = false;
	}
}

void XaPlayer::ApplyVolume()
{
	if(FAILED(voice->SetVolume(volume)))
		THROW("Can't apply volume to XAudio2 source voice");
}

void XaPlayer::ApplyPitch()
{
	if(FAILED(voice->SetFrequencyRatio(pitch)))
		THROW("Can't apply pitch to XAudio2 source voice");
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
	files.pop();
	if(files.empty())
		playing = false;
}

void CALLBACK XaPlayer::OnLoopEnd(void* pBufferContext)
{}

void CALLBACK XaPlayer::OnVoiceError(void* pBufferContext, HRESULT Error)
{}

END_INANITY_AUDIO
