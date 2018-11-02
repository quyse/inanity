#include "XaBufferedPlayer.hpp"
#include "XaSystem.hpp"
#include "XaDevice.hpp"
#include "XaBufferedSound.hpp"
#include "../File.hpp"

BEGIN_INANITY_AUDIO

XaBufferedPlayer::XaBufferedPlayer(ptr<XaBufferedSound> sound)
: XaPlayer(sound->GetDevice()), sound(sound)
{}

void XaBufferedPlayer::Play(bool looped)
{
	if(!voice)
	{
		SetVoice(sound->GetDevice()->GetSystem()->AllocateSourceVoice(sound->GetFormat(), this));
		PushBuffer(sound->GetFile());
	}

	StartPlaying();
}

void XaBufferedPlayer::Pause()
{
	StopPlaying();
}

void XaBufferedPlayer::Stop()
{
	if(voice)
	{
		StopPlaying();
		SetVoice(nullptr);
		while(!files.empty())
			files.pop();
	}
}

END_INANITY_AUDIO
