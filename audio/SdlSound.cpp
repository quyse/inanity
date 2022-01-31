#include "SdlSound.hpp"
#include "SdlDevice.hpp"
#include "SdlPlayer.hpp"
#include "Source.hpp"

BEGIN_INANITY_AUDIO

SdlSound::SdlSound(ptr<SdlDevice> device, ptr<Source> source)
: device(device), source(source)
{
}

ptr<Player3D> SdlSound::CreatePlayer3D()
{
  return NEW(SdlPlayer(device, source));
}

END_INANITY_AUDIO
