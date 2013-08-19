#ifndef ___INANITY_AUDIO_FORMAT_HPP___
#define ___INANITY_AUDIO_FORMAT_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// This struct represents format of a sound.
struct Format
{
	int samplesPerSecond;
	char bitsPerSample;
	char channelsCount;

	/// Operator which is needed for hashing.
	operator size_t() const
	{
		return samplesPerSecond | (bitsPerSample << 20) | (channelsCount << 26);
	}
};

END_INANITY_AUDIO

#endif
