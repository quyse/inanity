#include "XaStreamedPlayer.hpp"
#include "XaSystem.hpp"
#include "XaDevice.hpp"
#include "XaStreamedSound.hpp"
#include "Source.hpp"
#include "../MemoryFile.hpp"
#include "../InputStream.hpp"

BEGIN_INANITY_AUDIO

XaStreamedPlayer::XaStreamedPlayer(ptr<XaStreamedSound> sound)
: XaPlayer(sound->GetDevice()), sound(sound), looping(false)
{
	// calculate size of one buffer in bytes
	Format format = sound->GetSource()->GetFormat();
	bufferSize =
		format.samplesPerSecond
		* (format.bitsPerSample / 8)
		* format.channelsCount
		* bufferTime / 1000;
}

void XaStreamedPlayer::FillBuffers()
{
	while(files.size() < buffersCount && FillBuffer());
}

bool XaStreamedPlayer::FillBuffer()
{
	if(!looping && !stream)
		return false;

	ptr<File> file = NEW(MemoryFile(bufferSize));
	uint8_t* data = (uint8_t*)file->GetData();
	uint8_t* dataPtr = data;
	size_t sizeToRead = bufferSize;

	do
	{
		if(looping && !stream)
		{
			stream = sound->GetSource()->CreateStream();
		}

		size_t read = stream->Read(dataPtr, sizeToRead);
		if(read < sizeToRead)
			stream = nullptr;
		dataPtr += read;
		sizeToRead -= read;
	}
	while(looping && sizeToRead > 0);

	if(data < dataPtr)
		PushBuffer(file->Slice(0, dataPtr - data));

	return data < dataPtr;
}

void XaStreamedPlayer::Tick()
{
	for(; onBufferEndCounter > 0; --onBufferEndCounter)
		if(!files.empty())
			files.pop();

	FillBuffers();

	if(files.empty())
		playing = false;
}

void XaStreamedPlayer::Play(bool looped)
{
	looping = looped;

	if(!voice)
	{
		SetVoice(sound->GetDevice()->GetSystem()->AllocateSourceVoice(sound->GetSource()->GetFormat(), this));

		if(!looping && !stream)
			stream = sound->GetSource()->CreateStream();

		FillBuffers();
	}

	StartPlaying();
}

void XaStreamedPlayer::Pause()
{
	StopPlaying();
}

void XaStreamedPlayer::Stop()
{
	if(voice)
	{
		StopPlaying();
		SetVoice(nullptr);
		stream = nullptr;
		while(!files.empty())
			files.pop();
	}
}

END_INANITY_AUDIO
