#ifndef ___INANITY_AUDIO_FORMAT_HPP___
#define ___INANITY_AUDIO_FORMAT_HPP___

#include "audio.hpp"

BEGIN_INANITY_AUDIO

/// Структура формата аудиоданных.
struct Format
{
	/// Количество семплов в секунду.
	int samplesPerSecond;
	/// Количество битов на семпл.
	char bitsPerSample;
	/// Количество каналов.
	char channelsCount;

	/// Оператор для хеширования.
	operator size_t() const
	{
		return samplesPerSecond | (bitsPerSample << 20) | (channelsCount << 26);
	}
};

END_INANITY_AUDIO

#endif
