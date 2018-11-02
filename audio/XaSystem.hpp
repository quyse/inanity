#ifndef ___INANITY_AUDIO_XA_SYSTEM_HPP___
#define ___INANITY_AUDIO_XA_SYSTEM_HPP___

#include "System.hpp"
#include "Format.hpp"
#include "xaudio2.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_AUDIO

class Device;
struct Format;
class XaSourceVoice;

/// Класс звуковой подсистемы для XAudio.
class XaSystem : public System
{
private:
	ComPointer<IXAudio2> xAudio2;

public:
	XaSystem();

	/// Получить voice нужного формата.
	IXAudio2SourceVoice* AllocateSourceVoice(const Format& format, IXAudio2VoiceCallback* callback);

	static WAVEFORMATEX ConvertFormat(const Format& format);

	//** System's methods.
	ptr<Device> CreateDefaultDevice();
	void Tick();
};

END_INANITY_AUDIO

#endif
