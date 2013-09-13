#include "XaBufferedPlayer.hpp"
#include "XaDevice.hpp"
#include "XaSourceVoice.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaBufferedPlayer::XaBufferedPlayer(ptr<XaDevice> device, const Format& format, ptr<File> file)
: XaPlayer(device, format), file(file) {}

void XaBufferedPlayer::Play(bool looped)
{
	voice->Push(file, true);
	voice->Play();
}

void XaBufferedPlayer::Pause()
{
	voice->Pause();
}

void XaBufferedPlayer::Stop()
{
	voice->Stop();
}

bool XaBufferedPlayer::IsPlaying() const
{
	return voice->IsPlaying();
}

END_INANITY_AUDIO
