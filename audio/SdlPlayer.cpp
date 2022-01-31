#include "SdlPlayer.hpp"
#include "SdlDevice.hpp"
#include "Source.hpp"
#include "../InputStream.hpp"
#include "../CriticalCode.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

SdlPlayer::SdlPlayer(ptr<SdlDevice> device, ptr<Source> source)
: device(device), source(source), format(source->GetFormat())
{
}

SdlPlayer::~SdlPlayer()
{
  CriticalCode code(criticalSection);

  Unregister();
}

void SdlPlayer::Register()
{
  if(registered) return;
  device->RegisterPlayer(this);
  registered = true;
}

void SdlPlayer::Unregister()
{
  if(!registered) return;
  device->UnregisterPlayer(this);
  registered = false;
}

Format const& SdlPlayer::GetFormat() const
{
  return format;
}

size_t SdlPlayer::Read(Math::vec2* data, size_t count)
{
  CriticalCode code(criticalSection);

  if(!playing) return 0;

  uint8_t sampleSize = format.bitsPerSample / 8;
  uint8_t tickSize = sampleSize * format.channelsCount;
  buffer.resize(count * tickSize);

  size_t readCount = 0;
  while(count)
  {
    size_t read = stream->Read(buffer.data(), count * tickSize);

    if(!read)
    {
      if(looped)
      {
        stream = source->CreateStream();
        continue;
      }
      else
      {
        playing = false;
        stream = nullptr;
        break;
      }
    }

    switch(format.bitsPerSample)
    {
    case 8:
      switch(format.channelsCount)
      {
      case 1:
        for(size_t i = 0; i < read; )
        {
          float sample = (float)(buffer.data()[i++] - 127) / 128;
          sample *= volume;
          *data++ += Math::vec2(sample * (pan * -0.5f + 0.5f), sample * (pan * 0.5f + 0.5f));
          --count;
          ++readCount;
        }
        break;
      case 2:
        for(size_t i = 0; i + 1 < read; )
        {
          float sample1 = (float)(buffer.data()[i++] - 127) / 128;
          float sample2 = (float)(buffer.data()[i++] - 127) / 128;
          *data++ += Math::vec2(sample1 * volume * (pan * -0.5f + 0.5f), sample2 * volume * (pan * 0.5f + 0.5f));
          --count;
          ++readCount;
        }
        break;
      default:
        THROW("Unsupported channels count");
      }
      break;
    case 16:
      switch(format.channelsCount)
      {
      case 1:
        for(size_t i = 0; i * sizeof(int16_t) < read; )
        {
          float sample = (float)(((int16_t*)buffer.data())[i++]) / 0x8000;
          sample *= volume;
          *data++ += Math::vec2(sample * (pan * -0.5f + 0.5f), sample * (pan * 0.5f + 0.5f));
          --count;
          ++readCount;
        }
        break;
      case 2:
        for(size_t i = 0; (i + 1) * sizeof(int16_t) < read; )
        {
          float sample1 = (float)(((int16_t*)buffer.data())[i++]) / 0x8000;
          float sample2 = (float)(((int16_t*)buffer.data())[i++]) / 0x8000;
          *data++ += Math::vec2(sample1 * volume * (pan * -0.5f + 0.5f), sample2 * volume * (pan * 0.5f + 0.5f));
          --count;
          ++readCount;
        }
        break;
      default:
        THROW("Unsupported channels count");
      }
      break;
    }
  }

  return readCount;
}

void SdlPlayer::Play(bool looped)
{
  CriticalCode code(criticalSection);

  if(playing) return;

  playing = true;
  this->looped = looped;
  if(!stream)
    stream = source->CreateStream();
  Register();
}

void SdlPlayer::Pause()
{
  CriticalCode code(criticalSection);

  playing = false;
}

void SdlPlayer::Stop()
{
  CriticalCode code(criticalSection);

  Unregister();
  playing = false;
  stream = nullptr;
}

bool SdlPlayer::IsPlaying() const
{
  CriticalCode code(criticalSection);

  return playing;
}

void SdlPlayer::SetVolume(float volume)
{
  this->volume = volume;
}

void SdlPlayer::SetPanning(float pan)
{
  this->pan = pan;
}

void SdlPlayer::SetPitch(float pitch)
{
  this->pitch = pitch;
}

void SdlPlayer::SetPosition(const Math::vec3& position)
{
  // not implemented
}

void SdlPlayer::SetDirection(const Math::vec3& direction)
{
  // not implemented
}

void SdlPlayer::SetVelocity(const Math::vec3& velocity)
{
  // not implemented
}

END_INANITY_AUDIO
