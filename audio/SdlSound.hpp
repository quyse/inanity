#ifndef ___INANITY_AUDIO_SDL_SOUND_HPP___
#define ___INANITY_AUDIO_SDL_SOUND_HPP___

#include "Sound.hpp"

BEGIN_INANITY_AUDIO

class SdlDevice;
class Source;

/// Sound for SDL sound system.
class SdlSound : public Sound
{
private:
  ptr<SdlDevice> device;
  ptr<Source> source;

public:
  SdlSound(ptr<SdlDevice> device, ptr<Source> source);

  ptr<Player3D> CreatePlayer3D();
};

END_INANITY_AUDIO

#endif
