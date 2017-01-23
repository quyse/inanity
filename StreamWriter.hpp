#ifndef ___INANITY_STREAM_WRITER_HPP___
#define ___INANITY_STREAM_WRITER_HPP___

#include "OutputStream.hpp"
#include "String.hpp"

BEGIN_INANITY

/// Класс записывателя в поток вывода.
/** По сути является просто вспомогательным объектом. Предоставляет
сервис записи в поток вывода бинарных простых типов данных.
*/
class StreamWriter : public OutputStream
{
private:
	ptr<OutputStream> stream;
	size_t written;

public:
	/// Создать записыватель.
	/**
	\param stream Поток вывода.
	*/
	StreamWriter(ptr<OutputStream> stream);

	/// Получить значение счётчика записываемых данных.
	size_t GetWrittenSize() const;

	// Prevent hiding `Write` methods from base class.
	using OutputStream::Write;
	/// Write any data.
	void Write(const void* data, size_t size);

	/// Записать простое данное в поток вывода.
	/**
	\tparam T Тип простого данного.
	*/
	template <typename T>
	void Write(const T& data)
	{
		Write(&data, sizeof(data));
	}
	
	/// Записать сокращённое число.
	/** Сокращенные числа записываются в особом формате (см. примечания к реализации),
	что обеспечивает сокращенный размер записи. */
	void WriteShortly(size_t data);

	/// Записать сокращённое 64-битное число.
	void WriteShortlyBig(bigsize_t data);

	/// Записать строку в поток вывода
	void WriteString(const String& data);

	/// Записать 0 или более байт, чтобы достичь необходимого выравнивания.
	/** Выравнивание должно быть степенью двойки!
	Для профилактики ошибок записываются байты 0xCC, а не нули. */
	void WriteGap(size_t alignment);
};

template <typename T>
StreamWriter& operator<<(StreamWriter& writer, const T& value)
{
	writer.Write(value);
	return writer;
}

END_INANITY

#endif
