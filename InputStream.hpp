#ifndef ___INANITY_INPUT_STREAM_HPP___
#define ___INANITY_INPUT_STREAM_HPP___

#include "Object.hpp"
#include "scripting_decl.hpp"

BEGIN_INANITY

class File;
template <typename T>
class Future;

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

	/// Считать данные из потока асинхронно.
	/** Семантика такая же, как у Read. Реализация по умолчанию
	работает синхронно через Read. */
	virtual ptr<Future<size_t> > ReadAsync(ptr<File> file);

	SCRIPTABLE_CLASS(InputStream);
};

END_INANITY

#endif
