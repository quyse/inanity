#ifndef ___INANITY_AUDIO_AL_DEVICE_HPP___
#define ___INANITY_AUDIO_AL_DEVICE_HPP___

#include "Device.hpp"
#include "al.hpp"

BEGIN_INANITY_AUDIO

/// OpenAL sound device.
class AlDevice : public Device
{
private:
	::ALCdevice* device;
	::ALCcontext* context;

public:
	AlDevice(::ALCdevice* device);
	~AlDevice();

	//** Device's methods.
	ptr<Sound> CreateBufferedSound(ptr<Source> source);
	ptr<Sound> CreateStreamedSound(ptr<Source> source);
};

END_INANITY_AUDIO

#endif
