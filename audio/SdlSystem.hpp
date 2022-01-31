#ifndef ___INANITY_AUDIO_SDL_SYSTEM_HPP___
#define ___INANITY_AUDIO_SDL_SYSTEM_HPP___

#include "System.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class Sdl;

END_INANITY_PLATFORM

BEGIN_INANITY_AUDIO

class SdlSystem : public System
{
private:
  ptr<Platform::Sdl> sdl;

public:
  SdlSystem();

  //** System's methods.
  ptr<Device> CreateDefaultDevice();
  void Tick();
};

END_INANITY_AUDIO

#endif
