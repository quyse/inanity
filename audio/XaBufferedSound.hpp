#ifndef ___INANITY_AUDIO_XA_BUFFERED_SOUND_HPP___
#define ___INANITY_AUDIO_XA_BUFFERED_SOUND_HPP___

#include "Sound.hpp"
#include "Format.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_AUDIO

class XaDevice;

/// Класс буферизированного звука XAudio.
class XaBufferedSound : public Sound
{
private:
	ptr<XaDevice> device;
	Format format;
	ptr<File> file;

public:
	XaBufferedSound(ptr<XaDevice> device, const Format& format, ptr<File> file);

	ptr<XaDevice> GetDevice() const;
	const Format& GetFormat() const;
	ptr<File> GetFile() const;

	// Sound's methods.
	ptr<Player3D> CreatePlayer3D() override;
};

END_INANITY_AUDIO

#endif
