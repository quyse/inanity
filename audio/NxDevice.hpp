#ifndef ___INANITY_AUDIO_NX_DEVICE_HPP___
#define ___INANITY_AUDIO_NX_DEVICE_HPP___

#include "Device.hpp"
#include <nn/audio.h>

BEGIN_INANITY_AUDIO

class NxSystem;

/// Nintendo Switch audio device.
class NxDevice : public Device
{
private:
	ptr<NxSystem> system;

public:
	NxDevice(ptr<NxSystem> system);

	ptr<NxSystem> GetSystem() const;

	// Device's methods.
	ptr<Sound> CreateBufferedSound(ptr<Source> source) override;
	ptr<Sound> CreateStreamedSound(ptr<Source> source) override;
	void SetListenerPosition(const Math::vec3& position) override;
	void SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up) override;
	void SetListenerVelocity(const Math::vec3& velocity) override;
};

END_INANITY_AUDIO

#endif
