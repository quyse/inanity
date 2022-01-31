#include "SdlDevice.hpp"
#include "SdlSystem.hpp"
#include "SdlSound.hpp"
#include "SdlPlayer.hpp"
#include "Source.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

SdlDevice::SdlDevice(ptr<SdlSystem> system, uint32_t samplesPerSecond, uint8_t channelsCount)
: system(system)
{
  BEGIN_TRY();

  SDL_AudioSpec spec, actualSpec;
  SDL_zero(spec);
  spec.freq = samplesPerSecond;
  spec.format = AUDIO_F32;
  spec.channels = channelsCount;
  spec.samples = 4096;
  spec.callback = &SdlDevice::StaticCallback;
  spec.userdata = this;

  deviceId = SDL_OpenAudioDevice(nullptr, 0, &spec, &actualSpec, 0);
  if(!deviceId)
    THROW_SECONDARY("Can't open audio device", Platform::Sdl::Error());

  SDL_PauseAudioDevice(deviceId, 0);

  END_TRY("Can't create SDL audio device");
}

SdlDevice::~SdlDevice()
{
  SDL_CloseAudioDevice(deviceId);
}

void SdlDevice::StaticCallback(void* userdata, uint8_t* stream, int len)
{
  ((SdlDevice*)userdata)->Callback((Math::vec2*)stream, len / (sizeof(float) * 2));
}

void SdlDevice::Callback(Math::vec2* data, size_t count)
{
  std::fill(data, data + count, Math::vec2());
  for(auto player : players)
  {
    player->Read(data, count);
  }
}

void SdlDevice::RegisterPlayer(SdlPlayer* player)
{
  players.insert(player);
}

void SdlDevice::UnregisterPlayer(SdlPlayer* player)
{
  players.erase(player);
}

ptr<Sound> SdlDevice::CreateBufferedSound(ptr<Source> source)
{
  return NEW(SdlSound(this, source));
}

ptr<Sound> SdlDevice::CreateStreamedSound(ptr<Source> source)
{
  return NEW(SdlSound(this, source));
}

void SdlDevice::SetListenerPosition(const Math::vec3& position)
{
  // not implemented
}

void SdlDevice::SetListenerOrientation(const Math::vec3& forward, const Math::vec3& up)
{
  // not implemented
}

void SdlDevice::SetListenerVelocity(const Math::vec3& velocity)
{
  // not implemented
}

END_INANITY_AUDIO
