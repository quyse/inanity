#include "XaPlayer.hpp"
#include "XaDevice.hpp"
#include "XaSourceVoice.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

XaPlayer::XaPlayer(ptr<XaDevice> device, const Format& format)
{
	BEGIN_TRY();

	voice = device->AllocateSourceVoice(format);
	voice->SetPlayer(this);

	END_TRY("Can't create XAudio2 player");
}

XaPlayer::~XaPlayer()
{
	voice->SetPlayer(0);
	voice->Release();
}

void XaPlayer::SetPosition(const Math::vec3& position)
{
}

void XaPlayer::SetDirection(const Math::vec3& direction)
{
}

void XaPlayer::SetVelocity(const Math::vec3& velocity)
{
}


END_INANITY_AUDIO
