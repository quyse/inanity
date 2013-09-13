#include "XaSourceVoice.hpp"
#include "XaDevice.hpp"
#include "XaSystem.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaSourceVoice::XaSourceVoice(ptr<XaDevice> device, const Format& format)
: device(device), format(format), voice(0), player(0), buffersCount(0), playing(false)
{
	BEGIN_TRY();

	XAUDIO2_SEND_DESCRIPTOR send;
	send.Flags = 0;
	send.pOutputVoice = device->GetMasteringVoice();
	XAUDIO2_VOICE_SENDS voiceSends;
	voiceSends.SendCount = 1;
	voiceSends.pSends = &send;

	if(FAILED(device->GetSystem()->GetInterface()->CreateSourceVoice(
		&voice, // new voice object
		&XaSystem::ConvertFormat(format), // format
		0, // flags
		XAUDIO2_DEFAULT_FREQ_RATIO, // max frequency ratio
		this, // callback interface
		&voiceSends, // send list
		NULL // effect chain
		)))
		THROW("Can't create source voice");

	END_TRY("Can't create XAudio2 source voice");
}

XaSourceVoice::~XaSourceVoice()
{
	if(voice)
	{
		Stop();
		voice->DestroyVoice();
	}
}

ptr<XaDevice> XaSourceVoice::GetDevice() const
{
	return device;
}

const Format& XaSourceVoice::GetFormat() const
{
	return format;
}

IXAudio2SourceVoice* XaSourceVoice::GetVoice() const
{
	return voice;
}

void XaSourceVoice::Release()
{
	Stop();

	device->ReleaseSourceVoice(this);

	// break circular dependency
	device = 0;
}

void XaSourceVoice::Grab(ptr<XaDevice> device)
{
	this->device = device;
}

void XaSourceVoice::SetPlayer(XaPlayer* player)
{
	this->player = player;
}

void XaSourceVoice::Push(ptr<File> file, bool last)
{
	XAUDIO2_BUFFER buffer;
	buffer.Flags = last ? XAUDIO2_END_OF_STREAM : 0;
	buffer.AudioBytes = (UINT32)file->GetSize();
	buffer.pAudioData = (BYTE*)file->GetData();
	buffer.PlayBegin = 0;
	buffer.PlayLength = 0; // means all buffer
	buffer.LoopBegin = 0;
	buffer.LoopLength = 0;
	buffer.LoopCount = 0;
	buffer.pContext = &*file;

	file->Reference();

	if(FAILED(voice->SubmitSourceBuffer(&buffer)))
	{
		file->Dereference();
		THROW("Can't push buffer to play");
	}
	buffersCount++;
}

void XaSourceVoice::Play()
{
	if(FAILED(voice->Start(0)))
		THROW("Can't play XAudio2 source voice");
	playing = true;
}

void XaSourceVoice::Pause()
{
	voice->Stop(0);
	playing = false;
}

void XaSourceVoice::Stop()
{
	voice->Stop(0);
	voice->FlushSourceBuffers();
	playing = false;
	buffersCount = 0;
}

bool XaSourceVoice::IsPlaying() const
{
	return playing;
}

void CALLBACK XaSourceVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{}

void CALLBACK XaSourceVoice::OnVoiceProcessingPassEnd()
{}

void CALLBACK XaSourceVoice::OnStreamEnd()
{}

void CALLBACK XaSourceVoice::OnBufferStart(void* pBufferContext)
{}

void CALLBACK XaSourceVoice::OnBufferEnd(void* pBufferContext)
{
	((File*)pBufferContext)->Dereference();
	if(!--buffersCount)
		playing = false;
}

void CALLBACK XaSourceVoice::OnLoopEnd(void* pBufferContext)
{}

void CALLBACK XaSourceVoice::OnVoiceError(void* pBufferContext, HRESULT Error)
{}

END_INANITY_AUDIO
