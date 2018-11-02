#include "XaBufferedSound.hpp"
#include "XaDevice.hpp"
#include "XaBufferedPlayer.hpp"
#include "../File.hpp"

BEGIN_INANITY_AUDIO

XaBufferedSound::XaBufferedSound(ptr<XaDevice> device, const Format& format, ptr<File> file)
: device(device), format(format), file(file)
{
}

ptr<XaDevice> XaBufferedSound::GetDevice() const
{
	return device;
}

const Format& XaBufferedSound::GetFormat() const
{
	return format;
}

ptr<File> XaBufferedSound::GetFile() const
{
	return file;
}

ptr<Player3D> XaBufferedSound::CreatePlayer3D()
{
	return NEW(XaBufferedPlayer(this));
}

END_INANITY_AUDIO
