#ifndef ___INANITY_AUDIO_PLAYER_HPP___
#define ___INANITY_AUDIO_PLAYER_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// Абстрактный класс плеера звука.
/** */
class Player : public Object
{
public:
	/// Запустить проигрывание звука.
	/** \param repeat Количество повторений. 0 - повторять бесконечно. */
	virtual void Play(int repeat = 1) = 0;

	/// Остановить проигрывание звука.
	virtual void Stop() = 0;

	/// Получить, проигрывается ли звук.
	virtual bool IsPlaying() const = 0;
};

END_INANITY_AUDIO

#endif
