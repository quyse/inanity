#ifndef ___INANITY_AUDIO_AL_STREAMED_SOUND_HPP___
#define ___INANITY_AUDIO_AL_STREAMED_SOUND_HPP___

#include "Sound.hpp"

BEGIN_INANITY_AUDIO

class AlDevice;
class Source;

/// OpenAL streamed sound class.
class AlStreamedSound : public Sound
{
private:
	ptr<AlDevice> device;
	ptr<Source> source;

public:
	AlStreamedSound(ptr<AlDevice> device, ptr<Source> source);

	//** Sound's methods.
	ptr<Player3D> CreatePlayer3D();
};

END_INANITY_AUDIO

#endif
