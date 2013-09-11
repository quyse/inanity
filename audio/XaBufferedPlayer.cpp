#include "XaBufferedPlayer.hpp"
#include "XaDevice.hpp"
#include "XaSourceVoice.hpp"
#include "../File.hpp"

BEGIN_INANITY_AUDIO

XaBufferedPlayer::XaBufferedPlayer(ptr<XaDevice> device, const Format& format, ptr<File> file)
: XaPlayer(device, format), file(file) {}

void XaBufferedPlayer::Play(bool looped)
{
	voice->GetVoice()->Start(0);
}

void XaBufferedPlayer::Pause()
{
	voice->GetVoice()->Stop(0);
}

void XaBufferedPlayer::Stop()
{
	voice->GetVoice()->Stop(0);
}

bool XaBufferedPlayer::IsPlaying() const
{
	return false;
}

END_INANITY_AUDIO
