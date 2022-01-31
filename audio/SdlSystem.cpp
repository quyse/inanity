#include "SdlSystem.hpp"
#include "SdlDevice.hpp"
#include "../platform/Sdl.hpp"

BEGIN_INANITY_AUDIO

SdlSystem::SdlSystem()
: sdl(Platform::Sdl::Get(SDL_INIT_AUDIO))
{
}

ptr<Device> SdlSystem::CreateDefaultDevice()
{
  return NEW(SdlDevice(this, 44100, 2));
}

void SdlSystem::Tick()
{
}

END_INANITY_AUDIO
