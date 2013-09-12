#ifndef ___INANITY_AUDIO_XA_SYSTEM_HPP___
#define ___INANITY_AUDIO_XA_SYSTEM_HPP___

#include "System.hpp"
#include "xaudio2.hpp"
#include "../ComPointer.hpp"
#include "../windows.hpp"

BEGIN_INANITY_AUDIO

class Device;
class Sound;
struct Format;

/// XAudio2 sound system.
class XaSystem : public System
{
private:
	ComPointer<IXAudio2> xAudio2;

public:
	XaSystem();
	~XaSystem();

	IXAudio2* GetInterface() const;

	static WAVEFORMATEX ConvertFormat(const Format& format);

	//*** System's methods.
	ptr<Device> CreateDefaultDevice();
	void Tick();
};

END_INANITY_AUDIO

#endif
