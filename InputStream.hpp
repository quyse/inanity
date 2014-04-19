#ifndef ___INANITY_INPUT_STREAM_HPP___
#define ___INANITY_INPUT_STREAM_HPP___

#include "Object.hpp"
#include "meta/decl.hpp"

BEGIN_INANITY

class File;

/// Абстрактный класс потока ввода.
/** Предназначен для считывания потоковых данных из различных источников. */
class InputStream : public Object
{
public:
	/// Считывает данные из потока.
	/** Возвращает размер считанных данных в байтах
	или 0, если достигнут конец потока.
	Считывается ровно столько данных,
	сколько задано, кроме случая, когда достигается
	конец потока. В этом случае считывается столько данных, сколько есть,
	при этом исключение не генерируется.
	\param data Указатель на буфер для считываемых данных.
	\param size Размер данных для считывания, в байтах.
	*/
	virtual size_t Read(void* data, size_t size) = 0;

	/// Пропустить заданное количество данных.
	/** Возвращает количество пропущенных данных, аналогично Read.
	Поток может эффективно реализовывать этот метод.
	Реализация по умолчанию просто читает данные в никуда. */
	virtual bigsize_t Skip(bigsize_t size);

	/// Read some data into memory file.
	ptr<File> Read(size_t size);

	META_DECLARE_CLASS(InputStream);
};

END_INANITY

#endif
