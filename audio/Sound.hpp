#ifndef ___INANITY_AUDIO_SOUND_HPP___
#define ___INANITY_AUDIO_SOUND_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

class Player;
class Player3D;

/// Абстрактный класс звука.
class Sound : public Object
{
public:
	/// Создать плеер для звука.
	virtual ptr<Player> CreatePlayer() = 0;
	/// Создать 3D-плеер для звука.
	virtual ptr<Player3D> CreatePlayer3D() = 0;
};

END_INANITY_AUDIO

#endif
