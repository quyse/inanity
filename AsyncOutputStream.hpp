#ifndef ___INANITY_ASYNC_OUTPUT_STREAM_HPP___
#define ___INANITY_ASYNC_OUTPUT_STREAM_HPP___

#include "Object.hpp"
#include "EventHandler.hpp"
#include "Exception.hpp"

BEGIN_INANITY

class File;

/// Абстрактный класс асинхронного потока вывода.
/** Исключения всегда бросаются асинхронно, через обработчик событий. */
class AsyncOutputStream : public Object
{
public:
	/// Структура с данными, которая выдаётся обработчику событий.
	struct EventData
	{
		/// Исключение, если была ошибка.
		ptr<Exception> exception;
	};

	typedef EventHandler<EventData> Handler;

private:
	ptr<Handler> handler;

public:
	AsyncOutputStream(ptr<Handler> handler);

	/// Открыт ли поток?
	virtual bool IsOpen() const = 0;

	/// Выполнить запись данных.
	/** Метод может вызываться в любое время, но на открытом потоке.
	 * Он просто добавляет отправляемые данные в очередь. */
	virtual void Write(ptr<File> file) = 0;
};

END_INANITY

#endif
