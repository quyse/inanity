#include "XaBufferedSound.hpp"
#include "XaSystem.hpp"
#include "XaDevice.hpp"
#include "XaBufferedPlayer.hpp"
#include "../File.hpp"

BEGIN_INANITY_AUDIO

XaBufferedSound::XaBufferedSound(ptr<XaDevice> device, const Format& format, ptr<File> file)
: device(device), format(format), file(file) {}

ptr<Player3D> XaBufferedSound::CreatePlayer3D()
{
	return NEW(XaBufferedPlayer(device, format, file));
}

END_INANITY_AUDIO
