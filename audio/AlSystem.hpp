#ifndef ___INANITY_AUDIO_AL_SYSTEM_HPP___
#define ___INANITY_AUDIO_AL_SYSTEM_HPP___

#include "System.hpp"

BEGIN_INANITY_AUDIO

/// Class of OpenAL sound system.
class AlSystem : public System
{
private:
public:
	static void CheckErrors(const char* primaryExceptionString = 0);

	//** System's methods.
	ptr<Device> CreateDefaultDevice();
	void Tick();
};

END_INANITY_AUDIO

#endif
