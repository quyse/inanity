#ifndef ___INANITY_AUDIO_AL_DEVICE_HPP___
#define ___INANITY_AUDIO_AL_DEVICE_HPP___

#include "Device.hpp"
#include "al.hpp"

BEGIN_INANITY_AUDIO

class AlSystem;
class AlBuffer;
struct Format;

/// OpenAL sound device.
class AlDevice : public Device
{
private:
	ptr<AlSystem> system;
	::ALCdevice* device;
	::ALCcontext* context;

public:
	AlDevice(ptr<AlSystem> system, ::ALCdevice* device);
	~AlDevice();

	ptr<AlSystem> GetSystem() const;

	//** Device's methods.
	ptr<Sound> CreateBufferedSound(ptr<Source> source) override;
	ptr<Sound> CreateStreamedSound(ptr<Source> source) override;
	void SetListenerPosition(const Math::vec3& position) override;
	void SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up) override;
	void SetListenerVelocity(const Math::vec3& velocity) override;
};

END_INANITY_AUDIO

#endif
