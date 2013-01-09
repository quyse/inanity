#ifndef ___INANITY_AUDIO_SYSTEM_HPP___
#define ___INANITY_AUDIO_SYSTEM_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// Абстрактный класс звуковой системы.
class System : public Object
{
public:
	/// Создать устройство вывода по умолчанию.
	virtual ptr<Device> CreateDefaultDevice() = 0;

	/// Создать звук из источника звуковых данных.
	virtual ptr<Sound> CreateSound(ptr<Source> source) = 0;
	/// Создать потоковый звук из источника звуковых данных.
	virtual ptr<Sound> CreateStreamedSound(pr<Source> source) = 0;
};

END_INANITY_AUDIO

#endif
