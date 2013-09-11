#ifndef ___INANITY_AUDIO_XA_DEVICE_HPP___
#define ___INANITY_AUDIO_XA_DEVICE_HPP___

#include "Device.hpp"
#include "Format.hpp"
#include "../ComPointer.hpp"
#include "xaudio2.hpp"
#include <unordered_map>

BEGIN_INANITY_AUDIO

class XaSystem;
class XaSourceVoice;

/// Класс звукового устройства XAudio2.
class XaDevice : public Device
{
private:
	ptr<XaSystem> system;
	IXAudio2MasteringVoice* voice;

	/// Source voices cache.
	typedef std::unordered_multimap<Format, ptr<XaSourceVoice> > Voices;
	Voices voices;

public:
	XaDevice(ptr<XaSystem> system, IXAudio2MasteringVoice* voice);
	~XaDevice();

	ptr<XaSystem> GetSystem() const;
	IXAudio2MasteringVoice* GetMasteringVoice() const;

	ptr<XaSourceVoice> AllocateSourceVoice(const Format& format);
	void ReleaseSourceVoice(ptr<XaSourceVoice> voice);

	//*** Device's methods.
	ptr<Sound> CreateBufferedSound(ptr<Source> source);
	ptr<Sound> CreateStreamedSound(ptr<Source> source);
};

END_INANITY_AUDIO

#endif
