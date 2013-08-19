#ifndef ___INANITY_AUDIO_DEVICE_HPP___
#define ___INANITY_AUDIO_DEVICE_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

class Source;
class Sound;

/// Abstract sound device class.
class Device : public Object
{
public:
	virtual ptr<Sound> CreateBufferedSound(ptr<Source> source) = 0;
	virtual ptr<Sound> CreateStreamedSound(ptr<Source> source) = 0;
};

END_INANITY_AUDIO

#endif
