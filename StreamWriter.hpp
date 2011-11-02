#ifndef ___INANITY_STREAM_WRITER_HPP___
#define ___INANITY_STREAM_WRITER_HPP___

#include "Object.hpp"
#include <string>

BEGIN_INANITY

class OutputStream;

/// Класс записывателя в поток вывода.
/** По сути является просто вспомогательным объектом. Предоставляет
сервис записи в поток вывода бинарных простых типов данных.
*/
class StreamWriter : public Object
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

	/// Записать произвольные данные.
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
	
	/// Записать сокращенное число.
	/** Сокращенные числа записываются в особом формате (см. примечания к реализации),
	что обеспечивает сокращенный размер записи. */
	void WriteShortly(unsigned data);

	/// Записать строку Unicode в поток вывода
	void Write(const std::wstring& data);

	/// Записать строку ASCII в поток вывода
	void Write(const std::string& data);

	/// Записать 0 или более байт, чтобы достичь необходимого выравнивания.
	/** Выравнивание должно быть степенью двойки!
	Для профилактики ошибок записываются байты 0xCC, а не нули. */
	void WriteGap(unsigned alignment);
};

END_INANITY

#endif
