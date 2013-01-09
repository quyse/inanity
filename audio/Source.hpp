#ifndef ___INANITY_AUDIO_SOURCE_HPP___
#define ___INANITY_AUDIO_SOURCE_HPP___

#include "Format.hpp"

BEGIN_INANITY

class InputStream;

END_INANITY

BEGIN_INANITY_AUDIO

/// Абстрактный класс источника звука.
class Source : public Object
{
public:
	/// Получить формат звуковых данных.
	virtual Format GetFormat() const = 0;
	/// Получить длину звука в семплах.
	virtual size_t GetSamplesCount() const = 0;
	/// Получить размер звуковых данных в байтах.
	size_t GetSize() const;

	/// Создать поток звуковых данных.
	/** Он будет выдавать данные с начала. */
	virtual ptr<InputStream> CreateStream() = 0;
};

END_INANITY_AUDIO

#endif
