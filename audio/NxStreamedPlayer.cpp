#include "NxStreamedPlayer.hpp"
#include "NxSystem.hpp"
#include "Source.hpp"
#include "../File.hpp"
#include "../InputStream.hpp"

BEGIN_INANITY_AUDIO

NxStreamedPlayer::NxStreamedPlayer(ptr<NxSystem> system, ptr<Source> source)
: NxPlayer(system, source->GetFormat()), source(source)
{
	// initialize buffer files
	Format format = source->GetFormat();
	size_t bufferSize =
		format.samplesPerSecond
		* (format.bitsPerSample / 8)
		* format.channelsCount
		* bufferTime / 1000;
	sampleSize = format.channelsCount * format.bitsPerSample / 8;

	for(size_t i = 0; i < buffersCount; ++i)
	{
		ptr<File> file = system->CreateBufferFile(bufferSize);
		bufferFiles[i] = file;

		nn::audio::WaveBuffer& buffer = buffers[i];
		buffer.buffer = file->GetData();
		buffer.size = file->GetSize();
		buffer.startSampleOffset = 0;
		buffer.endSampleOffset = buffer.size / sampleSize;
		buffer.isEndOfStream = false;
		buffer.loop = false;
		buffer.loopCount = 0;
		buffer.loopStartSampleOffset = buffer.startSampleOffset;
		buffer.loopEndSampleOffset = buffer.endSampleOffset;
		buffer.pContext = nullptr;
		buffer.contextSize = 0;

		QueueWaveBuffer(&buffer);
	}
}

bool NxStreamedPlayer::RequestWaveBuffer(const nn::audio::WaveBuffer* buffer)
{
	size_t bufferIndex = buffer - buffers;

	if(!stream)
		stream = source->CreateStream();

	ptr<File> file = bufferFiles[bufferIndex];
	size_t read = stream->Read(file->GetData(), file->GetSize());
	if(read <= 0)
		return false;

	buffers[bufferIndex].endSampleOffset = read / sampleSize;

	AppendWaveBuffer(&buffers[bufferIndex]);

	return true;
}

void NxStreamedPlayer::Reset()
{
	stream = nullptr;
}

END_INANITY_AUDIO
