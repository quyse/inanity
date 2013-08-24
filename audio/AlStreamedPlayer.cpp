#include "AlStreamedPlayer.hpp"
#include "AlDevice.hpp"
#include "AlSystem.hpp"
#include "Source.hpp"
#include "AlBuffer.hpp"
#include "../InputStream.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

AlStreamedPlayer::AlStreamedPlayer(ptr<AlDevice> device, ptr<Source> source)
: AlPlayer(device), source(source), playing(false)
{
	// calculate size of one buffer in bytes
	Format format = source->GetFormat();
	bufferSize =
		format.samplesPerSecond
		* (format.bitsPerSample / 8)
		* format.channelsCount
		* bufferTime / 1000;

	device->GetSystem()->RegisterStreamedPlayer(this);
}

AlStreamedPlayer::~AlStreamedPlayer()
{
	Stop();
	device->GetSystem()->UnregisterStreamedPlayer(this);
}

size_t AlStreamedPlayer::Fill(ptr<AlBuffer>& buffer)
{
	if(!buffer)
	{
		ALuint bufferName;
		alGenBuffers(1, &bufferName);
		AlSystem::CheckErrors("Can't gen buffer");
		buffer = NEW(AlBuffer(device, bufferName));
	}

	void* data = alloca(bufferSize);
	size_t read = stream->Read(data, bufferSize);
	if(read < bufferSize)
		stream = 0;

	if(read)
	{
		Format format = source->GetFormat();
		alBufferData(
			buffer->GetName(),
			AlSystem::ConvertFormat(format),
			data, read,
			format.samplesPerSecond
		);
		AlSystem::CheckErrors("Can't upload data to buffer");
	}

	return read;
}

void AlStreamedPlayer::Play(int repeat)
{
	// get a stream
	if(!stream)
		stream = source->CreateStream();

	// set playing state
	playing = true;
}

void AlStreamedPlayer::Process()
{
	ALint buffersProcessed;
	alGetSourcei(sourceName, AL_BUFFERS_PROCESSED, &buffersProcessed);
	AlSystem::CheckErrors("Can't get number of processed buffers");

	if(buffersProcessed)
	{
		// unqueue processed buffers
		ALuint buffersToUnqueue[buffersCount];
		for(int i = 0; i < buffersProcessed; ++i)
			buffersToUnqueue[i] = buffers[i]->GetName();
		alSourceUnqueueBuffers(sourceName, buffersProcessed, buffersToUnqueue);

		// get processed buffers
		ptr<AlBuffer> freeBuffers[buffersCount];
		std::copy(buffers, buffers + buffersProcessed, freeBuffers);

		// move queued buffers to begin
		std::copy(buffers + buffersProcessed, buffers + buffersCount, buffers);

		// copy processed buffers to the end
		std::copy(freeBuffers, freeBuffers + buffersProcessed, buffers + (buffersCount - buffersProcessed));
	}

	// if there is a stream, and some free buffers, upload and queue data
	if(stream)
	{
		ALint buffersQueued;
		alGetSourcei(sourceName, AL_BUFFERS_QUEUED, &buffersQueued);
		AlSystem::CheckErrors("Can't get number of queued buffers");

		ALint initialBuffersQueued = buffersQueued;

		while(buffersQueued < buffersCount && stream)
			Fill(buffers[buffersQueued++]);

		// queue new buffers
		if(buffersQueued > initialBuffersQueued)
		{
			ALuint bufferNames[buffersCount];
			for(int i = initialBuffersQueued; i < buffersQueued; ++i)
				bufferNames[i] = buffers[i]->GetName();
			alSourceQueueBuffers(
				sourceName,
				buffersQueued - initialBuffersQueued,
				bufferNames + initialBuffersQueued
			);
		}

		// ensure playing
		if(playing)
		{
			ALint state;
			alGetSourcei(sourceName, AL_SOURCE_STATE, &state);
			AlSystem::CheckErrors("Can't get OpenAL streamed player playing state");

			if(state != AL_PLAYING)
			{
				alSourcePlay(sourceName);
				AlSystem::CheckErrors("Can't play source");
			}
		}
	}
}

void AlStreamedPlayer::Pause()
{
	alSourcePause(source);
	AlSystem::CheckErrors("Can't pause OpenAL streamed player");
	playing = false;
}

void AlStreamedPlayer::Stop()
{
	stream = 0;
	alSourceStop(source);
	AlSystem::CheckErrors("Can't stop OpenAL streamed player");
	playing = false;
}

bool AlStreamedPlayer::IsPlaying() const
{
	if(!playing)
		return false;

	ALint state;
	alGetSourcei(sourceName, AL_SOURCE_STATE, &state);
	AlSystem::CheckErrors("Can't get OpenAL streamed player playing state");
	return state == AL_PLAYING ||
		(state == AL_INITIAL || state == AL_STOPPED) && stream;
}

END_INANITY_AUDIO
