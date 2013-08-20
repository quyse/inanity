#include "AlDevice.hpp"
#include "AlBufferedSound.hpp"
#include "AlSystem.hpp"
#include "Source.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_AUDIO

AlDevice::AlDevice(ALCdevice* device) : device(device)
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

ptr<Sound> AlDevice::CreateBufferedSound(ptr<Source> source)
{
	BEGIN_TRY();

	// create a buffer
	ALuint buffer;
	alGenBuffers(1, &buffer);
	AlSystem::CheckErrors("Can't gen buffer");

	Format format = source->GetFormat();

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

	ptr<File> file = source->GetData();

	// fill it with data
	alBufferData(buffer, bufferFormat, file->GetData(), file->GetSize(), format.samplesPerSecond);
	AlSystem::CheckErrors("Can't upload buffer data");

	return NEW(AlBufferedSound(this, buffer));

	END_TRY("Can't create OpenAL buffered sound");
}

ptr<Sound> AlDevice::CreateStreamedSound(ptr<Source> source)
{
	THROW("Not implemented");
}

END_INANITY_AUDIO
