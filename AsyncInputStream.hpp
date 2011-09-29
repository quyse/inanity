#ifndef ___INANITY_ASYNC_INPUT_STREAM_HPP___
#define ___INANITY_ASYNC_INPUT_STREAM_HPP___

#include "Object.hpp"
#include "EventHandler.hpp"

BEGIN_INANITY

class File;
class Exception;

/// Абстрактный класс асинхронного потока ввода.
class AsyncInputStream : public Object
{
public:
	/// Структура с данными, выдаваемая обработчику событий.
	struct EventData
	{
		/// Исключение, или null, если ошибки не было.
		ptr<Exception> exception;
		/// Указатель на данные, или null, если данных нет.
		/** Если exception==null, и data=null, то значит, достигнут конец потока ввода. */
		void* data;
		/// Размер данных.
		size_t size;
	};

	typedef EventHandler<EventData> Handler;

private:
	ptr<Handler> handler;

public:
	AsyncInputStream(ptr<Handler> handler);

	/// Создать асинхронный поток ввода.
	AsyncInputStream();

	/// Приостановить ввод.
	virtual void Pause() = 0;
	/// Возобновить ввод.
	virtual void Resume() = 0;
};

END_INANITY

#endif
