#ifndef ___INANITY_AUDIO_XA_DEVICE_HPP___
#define ___INANITY_AUDIO_XA_DEVICE_HPP___

#include "Device.hpp"
#include "xaudio2.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_AUDIO

class XaSystem;

/// Класс звукового устройства XAudio2.
class XaDevice : public Device
{
private:
	ptr<XaSystem> system;
	IXAudio2MasteringVoice* voice;
	DWORD channelMask;
	uint32_t channelsCount;

public:
	XaDevice(ptr<XaSystem> system, IXAudio2MasteringVoice* voice);
	~XaDevice();

	XaSystem* GetSystem() const;
	DWORD GetChannelMask() const;
	uint32_t GetChannelsCount() const;

	//** Device's methods.
	ptr<Sound> CreateBufferedSound(ptr<Source> source) override;
	ptr<Sound> CreateStreamedSound(ptr<Source> source) override;
	void SetListenerPosition(const Math::vec3& position) override;
	void SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up) override;
	void SetListenerVelocity(const Math::vec3& velocity) override;
};

END_INANITY_AUDIO

#endif
