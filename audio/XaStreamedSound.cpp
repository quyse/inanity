#include "XaStreamedSound.hpp"
#include "XaDevice.hpp"
#include "XaStreamedPlayer.hpp"
#include "Source.hpp"

BEGIN_INANITY_AUDIO

XaStreamedSound::XaStreamedSound(ptr<XaDevice> device, ptr<Source> source)
: device(device), source(source)
{
}

ptr<XaDevice> XaStreamedSound::GetDevice() const
{
	return device;
}

ptr<Source> XaStreamedSound::GetSource() const
{
	return source;
}

ptr<Player3D> XaStreamedSound::CreatePlayer3D()
{
	return NEW(XaStreamedPlayer(this));
}

END_INANITY_AUDIO
