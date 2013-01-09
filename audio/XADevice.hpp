#ifndef ___INANITY_AUDIO_XA_DEVICE_HPP___
#define ___INANITY_AUDIO_XA_DEVICE_HPP___

#include "Device.hpp"

BEGIN_INANITY_AUDIO

/// Класс звукового устройства XAudio2.
class XADevice : public Device
{
private:
	ComPointer<IXAudio2MasteringVoice> voice;

public:
	XADevice(ComPointer<IXAudio2MasteringVoice> voice);
};

END_INANITY_AUDIO

#endif
