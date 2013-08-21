#include "AlBuffer.hpp"
#include "AlDevice.hpp"

BEGIN_INANITY_AUDIO

AlBuffer::AlBuffer(ptr<AlDevice> device, ALuint buffer)
: device(device), buffer(buffer) {}

AlBuffer::~AlBuffer()
{
	alDeleteBuffers(1, &buffer);
}

ptr<AlDevice> AlBuffer::GetDevice() const
{
	return device;
}

ALuint AlBuffer::GetBuffer() const
{
	return buffer;
}

END_INANITY_AUDIO
