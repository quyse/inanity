#ifndef ___INANITY_AUDIO_AL_BUFFERED_SOUND_HPP___
#define ___INANITY_AUDIO_AL_BUFFERED_SOUND_HPP___

#include "Sound.hpp"

BEGIN_INANITY_AUDIO

class AlBuffer;

/// Buffered sound for OpenAL sound system.
class AlBufferedSound : public Sound
{
private:
	ptr<AlBuffer> buffer;

public:
	AlBufferedSound(ptr<AlBuffer> buffer);

	ptr<AlBuffer> GetBuffer() const;

	ptr<Player> CreatePlayer();
	ptr<Player3D> CreatePlayer3D();
};

END_INANITY_AUDIO

#endif
