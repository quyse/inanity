#ifndef ___INANITY_AUDIO_DEVICE_HPP___
#define ___INANITY_AUDIO_DEVICE_HPP___

#include "audio.hpp"
#include "../math/basic.hpp"

BEGIN_INANITY_AUDIO

class Source;
class Sound;

/// Abstract sound device class.
class Device : public Object
{
public:
	virtual ptr<Sound> CreateBufferedSound(ptr<Source> source) = 0;
	virtual ptr<Sound> CreateStreamedSound(ptr<Source> source) = 0;

	virtual void SetListenerPosition(const Math::vec3& position) = 0;
	virtual void SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up) = 0;
	virtual void SetListenerVelocity(const Math::vec3& velocity) = 0;
};

END_INANITY_AUDIO

#endif
