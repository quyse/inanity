#ifndef ___INANITY_AUDIO_SOUND_HPP___
#define ___INANITY_AUDIO_SOUND_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

class Player;
class Player3D;

/// Abstract class representing sound.
/** This is a chunk of sound, which can be played by creation
a Player (or Player3D). */
class Sound : public Object
{
public:
	virtual ptr<Player> CreatePlayer();
	virtual ptr<Player3D> CreatePlayer3D() = 0;
};

END_INANITY_AUDIO

#endif
