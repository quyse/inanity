#ifndef ___INANITY_AUDIO_SOURCE_HPP___
#define ___INANITY_AUDIO_SOURCE_HPP___

#include "Format.hpp"

BEGIN_INANITY

class InputStream;

END_INANITY

BEGIN_INANITY_AUDIO

/// Абстрактный класс источника звука.
/** Источники звука не зависимы от звуковой системы.
Источник звука должен реализовывать оба метода доступа к данным
(GetData() и CreateStream()) для эффективности.
*/
class Source : public Object
{
public:
	/// Получить формат звуковых данных.
	virtual Format GetFormat() const = 0;
	/// Получить длину звука в семплах.
	virtual size_t GetSamplesCount() const = 0;
	/// Получить размер звуковых данных в байтах.
	size_t GetSize() const;

	/// Получить звуковые данные в виде файла.
	virtual ptr<File> GetData() = 0;
	/// Создать поток звуковых данных.
	/** Он будет выдавать данные с начала. */
	virtual ptr<InputStream> CreateStream() = 0;
};

END_INANITY_AUDIO

#endif
