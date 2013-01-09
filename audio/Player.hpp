#ifndef ___INANITY_AUDIO_PLAYER_HPP___
#define ___INANITY_AUDIO_PLAYER_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// Абстрактный класс плеера звука.
class Player : public Object
{
public:
	virtual void Play() = 0;
	virtual void PlayCycled() = 0;
	virtual void Stop() = 0;
	virtual bool IsPlaying() const = 0;
};

END_INANITY_AUDIO

#endif
