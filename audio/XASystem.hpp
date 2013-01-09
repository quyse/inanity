#ifndef ___INANITY_AUDIO_XA_SYSTEM_HPP___
#define ___INANITY_AUDIO_XA_SYSTEM_HPP___

#include "System.hpp"
#include "../ComPointer.hpp"
#include "../windows.hpp"

BEGIN_INANITY_AUDIO

class Device;
struct Format;

/// Класс звуковой подсистемы для XAudio.
class XASystem : public System
{
private:
	ComPointer<IXAudio2> xAudio2;

public:
	XASystem();

	static WAVEFORMATEX ConvertFormat(const Format& format);

	ptr<Device> CreateDefaultDevice();
	ptr<Sound> CreateSound(ptr<Source> source);
	ptr<Sound> CreateStreamedSound(ptr<Source> source);
};

END_INANITY_AUDIO

#endif
