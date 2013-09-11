#include "XaDevice.hpp"
#include "XaSystem.hpp"
#include "XaSourceVoice.hpp"
#include "XaBufferedSound.hpp"
#include "Source.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaDevice::XaDevice(ptr<XaSystem> system, IXAudio2MasteringVoice* voice)
: system(system), voice(voice) {}

XaDevice::~XaDevice()
{
	voice->DestroyVoice();
}

ptr<XaSystem> XaDevice::GetSystem() const
{
	return system;
}

IXAudio2MasteringVoice* XaDevice::GetMasteringVoice() const
{
	return voice;
}

ptr<XaSourceVoice> XaDevice::AllocateSourceVoice(const Format& format)
{
	// try to find existing voice
	Voices::iterator i = voices.find(format);
	if(i != voices.end())
	{
		ptr<XaSourceVoice> voice = i->second;
		voices.erase(i);
		voice->Grab(this);
		return voice;
	}

	// create new voice
	return NEW(XaSourceVoice(this, format));
}

void XaDevice::ReleaseSourceVoice(ptr<XaSourceVoice> voice)
{
	voices.insert(std::make_pair(voice->GetFormat(), voice));
}

ptr<Sound> XaDevice::CreateBufferedSound(ptr<Source> source)
{
	return NEW(XaBufferedSound(this, source->GetFormat(), source->GetData()));
}

ptr<Sound> XaDevice::CreateStreamedSound(ptr<Source> source)
{
	THROW("Not implemented");
}

END_INANITY_AUDIO
