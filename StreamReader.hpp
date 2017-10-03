#ifndef ___INANITY_STREAM_READER_HPP___
#define ___INANITY_STREAM_READER_HPP___

#include "InputStream.hpp"
#include "String.hpp"

BEGIN_INANITY

class InputStream;

/// Класс читателя из потока ввода.
/** По сути является просто вспомогательным объектом. Предоставляет
сервис чтения из потока ввода бинарных простых типов данных.
Читатель ужесточает семантику InputStream - всё, что запрашивается,
должно считываться целиком, если возникает конец потока, выбрасывается
исключение.
*/
class StreamReader : public InputStream
{
private:
	ptr<InputStream> stream;
	bigsize_t read;

public:
	/// Создать читатель.
	/**
	\param stream Поток ввода.
	*/
	StreamReader(ptr<InputStream> stream);

	/// Получить значение счётчика считываемых данных.
	bigsize_t GetReadSize() const;

	// Prevent hiding `Read` methods from base class.
	using InputStream::Read;
	/// Read any data.
	size_t Read(void* data, size_t size);

	/// Прочитать простое данное из потока ввода.
	/**
	\tparam T Тип простого данного.
	\return Считанное данное.
	*/
	template <typename T>
	T Read()
	{
		T data;
		Read(&data, sizeof(data));
		return data;
	}

	/// Считать строку из потока ввода
	String ReadString(size_t maximumLength = 0xffff);

	/// Считать сокращённое число.
	/** Сокращенные числа записываются в особом формате (см. примечания к реализации),
	что обеспечивает сокращенный размер записи. */
	size_t ReadShortly();

	/// Считать сокращённое 64-битное число.
	bigsize_t ReadShortlyBig();

	/// Пропустить 0 или более байт, чтобы достичь необходимого выравнивания.
	/** Выравнивание должно быть степенью двойки!
	Сами байты не считываются. */
	void ReadGap(size_t alignment);

	/// Check that end of stream reached.
	/** Throw if not. */
	void ReadEnd();

	/// Пропустить заданное количество байт.
	bigsize_t Skip(bigsize_t size);

	bool IsAtEnd() const;
};

template <typename T>
StreamReader& operator>>(StreamReader& reader, T& value)
{
	reader.Read(value);
	return reader;
}


END_INANITY

#endif
