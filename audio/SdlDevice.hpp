#ifndef ___INANITY_AUDIO_SDL_DEVICE_HPP___
#define ___INANITY_AUDIO_SDL_DEVICE_HPP___

#include "Device.hpp"
#include "../platform/Sdl.hpp"
#include <unordered_set>

BEGIN_INANITY_AUDIO

class SdlSystem;
class SdlPlayer;

class SdlDevice : public Device
{
private:
  ptr<SdlSystem> system;
  SDL_AudioDeviceID deviceId;
  using Players = std::unordered_set<SdlPlayer*>;
  Players players;

  static void StaticCallback(void* userdata, uint8_t* stream, int len);
  void Callback(Math::vec2* data, size_t count);

public:
  SdlDevice(ptr<SdlSystem> system, uint32_t samplesPerSecond, uint8_t channelsCount);
  ~SdlDevice();

  void RegisterPlayer(SdlPlayer* player);
  void UnregisterPlayer(SdlPlayer* player);

  //** Device's methods.
  ptr<Sound> CreateBufferedSound(ptr<Source> source) override;
  ptr<Sound> CreateStreamedSound(ptr<Source> source) override;
  void SetListenerPosition(const Math::vec3& position) override;
  void SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up) override;
  void SetListenerVelocity(const Math::vec3& velocity) override;
};

END_INANITY_AUDIO

#endif
