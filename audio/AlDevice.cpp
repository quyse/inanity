#include "AlDevice.hpp"
#include "AlSystem.hpp"
#include "AlBuffer.hpp"
#include "AlBufferedSound.hpp"
#include "Source.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

AlDevice::AlDevice(ptr<AlSystem> system, ALCdevice* device)
: system(system), device(device)
{
	context = alcCreateContext(device, 0);
	if(!context)
		THROW("Can't create context");

	alcMakeContextCurrent(context);
	AlSystem::CheckErrors("Can't make context current");
}

AlDevice::~AlDevice()
{
	if(context)
	{
		alcMakeContextCurrent(0);
		alcDestroyContext(context);
	}
	if(device)
		alcCloseDevice(device);
}

ptr<AlSystem> AlDevice::GetSystem() const
{
	return system;
}

ptr<AlBuffer> AlDevice::CreateBuffer(const Format& format, const void* data, size_t size)
{
	ALenum bufferFormat;
	switch(format.channelsCount)
	{
	case 1:
		switch(format.bitsPerSample)
		{
		case 8: bufferFormat = AL_FORMAT_MONO8; break;
		case 16: bufferFormat = AL_FORMAT_MONO16; break;
		default: THROW("Unsupported bits per sample");
		}
		break;
	case 2:
		switch(format.bitsPerSample)
		{
		case 8: bufferFormat = AL_FORMAT_STEREO8; break;
		case 16: bufferFormat = AL_FORMAT_STEREO16; break;
		default: THROW("Unsupported bits per sample");
		}
		break;
	default:
		THROW("Unsupported number of channels");
	}

	// create a buffer
	ALuint bufferName;
	alGenBuffers(1, &bufferName);
	AlSystem::CheckErrors("Can't gen buffer");
	ptr<AlBuffer> buffer = NEW(AlBuffer(this, bufferName));

	// fill it with data
	alBufferData(bufferName, bufferFormat, data, size, format.samplesPerSecond);
	AlSystem::CheckErrors("Can't upload buffer data");

	return buffer;
}

ptr<Sound> AlDevice::CreateBufferedSound(ptr<Source> source)
{
	BEGIN_TRY();

	ptr<File> file = source->GetData();

	return NEW(AlBufferedSound(CreateBuffer(source->GetFormat(), file->GetData(), file->GetSize())));

	END_TRY("Can't create OpenAL buffered sound");
}

ptr<Sound> AlDevice::CreateStreamedSound(ptr<Source> source)
{
	THROW("Not implemented");
}

END_INANITY_AUDIO
