#include "XaDevice.hpp"
#include "XaSystem.hpp"
#include "XaBufferedSound.hpp"
#include "XaStreamedSound.hpp"
#include "Source.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaDevice::XaDevice(ptr<XaSystem> system, IXAudio2MasteringVoice* voice)
: system(system), voice(voice)
{
	if(FAILED(voice->GetChannelMask(&channelMask)))
		THROW("Can't get XAudio2 mastering voice channel mask");

	XAUDIO2_VOICE_DETAILS details;
	voice->GetVoiceDetails(&details);
	channelsCount = details.InputChannels;
}

XaDevice::~XaDevice()
{
	voice->DestroyVoice();
}

XaSystem* XaDevice::GetSystem() const
{
	return system;
}

DWORD XaDevice::GetChannelMask() const
{
	return channelMask;
}

uint32_t XaDevice::GetChannelsCount() const
{
	return channelsCount;
}

ptr<Sound> XaDevice::CreateBufferedSound(ptr<Source> source)
{
	return NEW(XaBufferedSound(this, source->GetFormat(), source->GetData()));
}

ptr<Sound> XaDevice::CreateStreamedSound(ptr<Source> source)
{
	return NEW(XaStreamedSound(this, source));
}

void XaDevice::SetListenerPosition(const Math::vec3& position)
{
	// not implemented yet
}

void XaDevice::SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up)
{
	// not implemented yet
}

void XaDevice::SetListenerVelocity(const Math::vec3& velocity)
{
	// not implemented yet
}

END_INANITY_AUDIO
