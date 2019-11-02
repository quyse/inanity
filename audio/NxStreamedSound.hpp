#ifndef ___INANITY_AUDIO_NX_STREAMED_SOUND_HPP___
#define ___INANITY_AUDIO_NX_STREAMED_SOUND_HPP___

#include "Sound.hpp"

BEGIN_INANITY_AUDIO

class NxSystem;
class Source;

/// Streamed sound for NX sound system.
class NxStreamedSound : public Sound
{
private:
	ptr<NxSystem> system;
	ptr<Source> source;

public:
	NxStreamedSound(ptr<NxSystem> system, ptr<Source> source);

	//*** Sound's methods.
	ptr<Player> CreatePlayer() override;
	ptr<Player3D> CreatePlayer3D() override;
};

END_INANITY_AUDIO

#endif
