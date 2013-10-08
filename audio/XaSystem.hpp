#ifndef ___INANITY_AUDIO_XA_SYSTEM_HPP___
#define ___INANITY_AUDIO_XA_SYSTEM_HPP___

#include "System.hpp"
#include "xaudio2.hpp"
#include "../ComPointer.hpp"
#include "../platform/windows.hpp"
#include <unordered_multimap>

BEGIN_INANITY_AUDIO

class Device;
struct Format;

/// Класс звуковой подсистемы для XAudio.
class XASystem : public System
{
private:
	ComPointer<IXAudio2> xAudio2;

	/// Внутренний кэш исходных voice по форматам.
	std::unordered_multimap<Format, ptr<XASourceVoice> > freeSourceVoices;

	/// Получить voice нужного формата.
	ptr<XASourceVoice> AllocateSourceVoice(const Format& format);

public:
	XASystem();

	static WAVEFORMATEX ConvertFormat(const Format& format);

	ptr<Device> CreateDefaultDevice();
	ptr<Sound> CreateBufferedSound(ptr<Source> source);
	ptr<Sound> CreateStreamedSound(ptr<Source> source);
	void Tick();
};

END_INANITY_AUDIO

#endif
