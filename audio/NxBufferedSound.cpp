#include "NxBufferedSound.hpp"
#include "NxBufferedPlayer.hpp"
#include "NxSystem.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

NxBufferedSound::NxBufferedSound(ptr<NxSystem> system, ptr<File> file, const Format& format)
: system(system), file(file), format(format)
{
	// init wave buffer
	buffer.buffer = file->GetData();
	buffer.size = file->GetSize();
	buffer.startSampleOffset = 0;
	buffer.endSampleOffset = file->GetSize() / (format.channelsCount * format.bitsPerSample / 8);
	buffer.isEndOfStream = true;
	buffer.loop = false;
	buffer.loopCount = 0;
	buffer.loopStartSampleOffset = buffer.startSampleOffset;
	buffer.loopEndSampleOffset = buffer.endSampleOffset;
	buffer.pContext = nullptr;
	buffer.contextSize = 0;
}

ptr<NxSystem> NxBufferedSound::GetSystem() const
{
	return system;
}

ptr<Player> NxBufferedSound::CreatePlayer()
{
	return NEW(NxBufferedPlayer(this, buffer, format));
}

ptr<Player3D> NxBufferedSound::CreatePlayer3D()
{
	THROW("Player3D is not supported");
}

END_INANITY_AUDIO
