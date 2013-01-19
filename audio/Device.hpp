#ifndef ___INANITY_AUDIO_DEVICE_HPP___
#define ___INANITY_AUDIO_DEVICE_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// Абстрактный класс устройства вывода звука.
class Device : public Object
{
public:
	/// Создать плейер буферизованного звука.
	virtual ptr<BufferedPlayer> CreateBufferedPlayer(ptr<Source> source) = 0;
	/// Создать плейер потокового звука.
	virtual ptr<StreamedPlayer> CreateStreamedPlayer(ptr<Source> source) = 0;
};

END_INANITY_AUDIO

#endif
