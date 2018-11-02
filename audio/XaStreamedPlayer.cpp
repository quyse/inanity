#include "XaStreamedPlayer.hpp"
#include "XaDevice.hpp"
#include "XaStreamedSound.hpp"

BEGIN_INANITY_AUDIO

XaStreamedPlayer::XaStreamedPlayer(ptr<XaStreamedSound> sound)
: XaPlayer(sound->GetDevice()), sound(sound)
{
}

void XaStreamedPlayer::Play(bool looped)
{
	// not implemented yet
}

void XaStreamedPlayer::Pause()
{
	// not implemented yet
}

void XaStreamedPlayer::Stop()
{
	// not implemented yet
}

END_INANITY_AUDIO
