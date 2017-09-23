#include "AlBuffer.hpp"
#include "AlDevice.hpp"
#include "AlSystem.hpp"

BEGIN_INANITY_AUDIO

AlBuffer::AlBuffer(ptr<AlDevice> device, ALuint bufferName)
: device(device), bufferName(bufferName) {}

AlBuffer::~AlBuffer()
{
	alDeleteBuffers(1, &bufferName);
	AlSystem::CheckErrors("Can't delete buffer");
}

ptr<AlDevice> AlBuffer::GetDevice() const
{
	return device;
}

ALuint AlBuffer::GetName() const
{
	return bufferName;
}

END_INANITY_AUDIO
