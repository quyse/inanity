#ifndef ___INANITY_AUDIO_NX_SYSTEM_HPP___
#define ___INANITY_AUDIO_NX_SYSTEM_HPP___

#include "System.hpp"

BEGIN_INANITY_AUDIO

class NxSystem : public System
{
public:
	// System's methods.
	ptr<Device> CreateDefaultDevice();
	void Tick();
};

END_INANITY_AUDIO

#endif
