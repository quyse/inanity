#ifndef ___INANITY_AUDIO_SDL_PLAYER_HPP___
#define ___INANITY_AUDIO_SDL_PLAYER_HPP___

#include "Player3D.hpp"
#include "Format.hpp"
#include "../CriticalSection.hpp"
#include <vector>

BEGIN_INANITY

class InputStream;

END_INANITY

BEGIN_INANITY_AUDIO

class SdlDevice;
class Source;

class SdlPlayer : public Player3D
{
private:
  ptr<SdlDevice> const device;
  ptr<Source> const source;
  Format const format;
  std::vector<uint8_t> buffer;

  mutable CriticalSection criticalSection;

  ptr<InputStream> stream;
  bool playing = false;
  bool looped = false;
  bool registered = false;
  float volume = 1;
  float pan = 0;
  float pitch = 1;

  void Register();
  void Unregister();

public:
  SdlPlayer(ptr<SdlDevice> device, ptr<Source> source);
  ~SdlPlayer();

  Format const& GetFormat() const;
  size_t Read(Math::vec2* data, size_t count);

  // Player's methods.
  void Play(bool looped = false) override;
  void Pause() override;
  void Stop() override;
  bool IsPlaying() const override;
  void SetVolume(float volume) override;
  void SetPanning(float pan) override;
  void SetPitch(float pitch) override;

  // Player3D's methods.
  void SetPosition(const Math::vec3& position) override;
  void SetDirection(const Math::vec3& direction) override;
  void SetVelocity(const Math::vec3& velocity) override;
};

END_INANITY_AUDIO

#endif
