#include "AlDevice.hpp"
#include "AlSystem.hpp"
#include "AlBuffer.hpp"
#include "AlBufferedSound.hpp"
#include "AlStreamedSound.hpp"
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

ptr<Sound> AlDevice::CreateBufferedSound(ptr<Source> source)
{
	BEGIN_TRY();

	// create a buffer
	ALuint bufferName;
	alGenBuffers(1, &bufferName);
	AlSystem::CheckErrors("Can't gen buffer");
	ptr<AlBuffer> buffer = NEW(AlBuffer(this, bufferName));

	ptr<File> file = source->GetData();

	Format format = source->GetFormat();

	// fill it with data
	alBufferData(
		bufferName,
		AlSystem::ConvertFormat(source->GetFormat()),
		file->GetData(),
		file->GetSize(),
		format.samplesPerSecond
	);
	AlSystem::CheckErrors("Can't upload buffer data");

	return NEW(AlBufferedSound(buffer));

	END_TRY("Can't create OpenAL buffered sound");
}

ptr<Sound> AlDevice::CreateStreamedSound(ptr<Source> source)
{
	return NEW(AlStreamedSound(this, source));
}

END_INANITY_AUDIO
