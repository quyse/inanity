#ifndef ___INANITY_AUDIO_AL_BUFFERED_SOUND_HPP___
#define ___INANITY_AUDIO_AL_BUFFERED_SOUND_HPP___

#include "Sound.hpp"
#include "al.hpp"

BEGIN_INANITY_AUDIO

class AlDevice;

/// Buffered sound for OpenAL sound system.
class AlBufferedSound : public Sound
{
private:
	ptr<AlDevice> device;
	ALuint buffer;

public:
	AlBufferedSound(ptr<AlDevice> device, ALuint buffer);
	~AlBufferedSound();

	ptr<AlDevice> GetDevice() const;
	ALuint GetBuffer() const;

	ptr<Player> CreatePlayer();
	ptr<Player3D> CreatePlayer3D();
};

END_INANITY_AUDIO

#endif
