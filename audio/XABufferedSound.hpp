#ifndef ___INANITY_AUDIO_XA_BUFFERED_SOUND_HPP___
#define ___INANITY_AUDIO_XA_BUFFERED_SOUND_HPP___

#include "audio.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

/// Класс буферизированного звука XAudio.
class XABufferedSound : public Sound
{
private:
	ptr<XASystem> system;
	ptr<File> file;

public:
	XABufferedSound(ptr<XASystem> system, ptr<File> file);

	ptr<Player> CreatePlayer();
	ptr<Player> CreatePlayer3D();
};

END_INANITY_AUDIO

#endif
