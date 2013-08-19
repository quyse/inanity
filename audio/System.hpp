#ifndef ___INANITY_AUDIO_SYSTEM_HPP___
#define ___INANITY_AUDIO_SYSTEM_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

class Device;
class Source;
class Sound;

/// Abstract class of sound system.
/** Sound system represents an API which is used to talk
with audio devices. */
class System : public Object
{
public:
	virtual ptr<Device> CreateDefaultDevice() = 0;

	/// Performs sound processing.
	/** Should be called periodically. */
	virtual void Tick() = 0;
};

END_INANITY_AUDIO

#endif
