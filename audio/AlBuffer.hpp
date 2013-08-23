#ifndef ___INANITY_AUDIO_AL_BUFFER_HPP___
#define ___INANITY_AUDIO_AL_BUFFER_HPP___

#include "audio.hpp"
#include "al.hpp"

BEGIN_INANITY_AUDIO

class AlDevice;

class AlBuffer : public Object
{
private:
	ptr<AlDevice> device;
	ALuint bufferName;

public:
	AlBuffer(ptr<AlDevice> device, ALuint bufferName);
	~AlBuffer();

	ptr<AlDevice> GetDevice() const;
	ALuint GetName() const;
};

END_INANITY_AUDIO

#endif
