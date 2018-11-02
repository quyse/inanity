#ifndef ___INANITY_AUDIO_XA_STREAMED_SOUND_HPP___
#define ___INANITY_AUDIO_XA_STREAMED_SOUND_HPP___

#include "Sound.hpp"
#include "Format.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

class XaDevice;
class Source;

/// Класс стримящегося звука XAudio.
class XaStreamedSound : public Sound
{
private:
	ptr<XaDevice> device;
	ptr<Source> source;

public:
	XaStreamedSound(ptr<XaDevice> device, ptr<Source> source);

	ptr<XaDevice> GetDevice() const;
	ptr<Source> GetSource() const;

	// Sound's methods.
	ptr<Player3D> CreatePlayer3D() override;
};

END_INANITY_AUDIO

#endif
