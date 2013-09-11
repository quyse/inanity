#include "XaSourceVoice.hpp"
#include "XaDevice.hpp"
#include "XaSystem.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaSourceVoice::XaSourceVoice(ptr<XaDevice> device, const Format& format)
: device(device), format(format), voice(0), player(0)
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
		voice->DestroyVoice();
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

void CALLBACK XaSourceVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{}

void CALLBACK XaSourceVoice::OnVoiceProcessingPassEnd()
{}

void CALLBACK XaSourceVoice::OnStreamEnd()
{}

void CALLBACK XaSourceVoice::OnBufferStart(void* pBufferContext)
{}

void CALLBACK XaSourceVoice::OnBufferEnd(void* pBufferContext)
{}

void CALLBACK XaSourceVoice::OnLoopEnd(void* pBufferContext)
{}

void CALLBACK XaSourceVoice::OnVoiceError(void* pBufferContext, HRESULT Error)
{}

END_INANITY_AUDIO
