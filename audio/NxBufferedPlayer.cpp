#include "NxBufferedPlayer.hpp"
#include "NxBufferedSound.hpp"
#include "NxSystem.hpp"

BEGIN_INANITY_AUDIO

NxBufferedPlayer::NxBufferedPlayer(ptr<NxBufferedSound> sound, const nn::audio::WaveBuffer& buffer, const Format& format)
: NxPlayer(sound->GetSystem(), format), sound(sound), buffer(buffer), bufferAdded(false)
{
	// add buffer to queue a few times
	for(size_t i = 0; i < 3; ++i)
		QueueWaveBuffer(&buffer);
}

bool NxBufferedPlayer::RequestWaveBuffer(const nn::audio::WaveBuffer*)
{
	if(bufferAdded) return false;

	AppendWaveBuffer(&buffer);

	bufferAdded = true;

	return true;
}

void NxBufferedPlayer::Reset()
{
	bufferAdded = false;
}

END_INANITY_AUDIO
