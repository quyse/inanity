#ifndef ___INANITY_AUDIO_NX_BUFFERED_SOUND_HPP___
#define ___INANITY_AUDIO_NX_BUFFERED_SOUND_HPP___

#include "Sound.hpp"
#include "Format.hpp"
#include <nn/audio.h>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

class NxSystem;

/// Buffered sound for NX sound system.
class NxBufferedSound : public Sound
{
private:
	ptr<NxSystem> system;
	ptr<File> file;
	Format format;
	nn::audio::WaveBuffer buffer;

public:
	NxBufferedSound(ptr<NxSystem> system, ptr<File> file, const Format& format);

	ptr<NxSystem> GetSystem() const;

	//*** Sound's methods.
	ptr<Player> CreatePlayer() override;
	ptr<Player3D> CreatePlayer3D() override;
};

END_INANITY_AUDIO

#endif
