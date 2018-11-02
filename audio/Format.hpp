#ifndef ___INANITY_AUDIO_FORMAT_HPP___
#define ___INANITY_AUDIO_FORMAT_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// This struct represents format of a sound.
struct Format
{
	uint32_t samplesPerSecond;
	uint8_t bitsPerSample;
	uint8_t channelsCount;

	friend bool operator==(const Format& a, const Format& b)
	{
		return
			a.samplesPerSecond == b.samplesPerSecond &&
			a.bitsPerSample == b.bitsPerSample &&
			a.channelsCount == b.channelsCount;
	}
};

END_INANITY_AUDIO

namespace std
{
	template <> struct hash<Inanity::Audio::Format>
	{
		typedef Inanity::Audio::Format argument_type;
		typedef size_t result_type;
		result_type operator()(const argument_type& s) const noexcept
		{
			return s.samplesPerSecond | (s.bitsPerSample << 20) | (s.channelsCount << 26);
		}
	};
}

#endif
