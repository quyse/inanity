#ifndef ___INANITY_ASYNC_OUTPUT_STREAM_HPP___
#define ___INANITY_ASYNC_OUTPUT_STREAM_HPP___

#include "Object.hpp"
#include "EventHandler.hpp"
#include <queue>

BEGIN_INANITY

class File;
class Exception;

/// Абстрактный класс асинхронного потока вывода.
/** Исключения всегда бросаются асинхронно, через обработчик событий. */
class AsyncOutputStream : public Object
{
public:
	/// Тип события.
	enum EventType
	{
		/// Сообщает, что очередь записи пуста.
		eventTypeFlushed,
		/// Произошла ошибка.
		eventTypeError
	};
	/// Структура с данными, которая выдаётся обработчику событий.
	struct EventData
	{
		/// Тип события.
		EventType type;
		/// Исключение, если была ошибка.
		ptr<Exception> exception;

		EventData(EventType type, ptr<Exception> exception = 0);
	};

	typedef EventHandler<EventData> Handler;

private:
	/// Обработчик событий.
	ptr<Handler> handler;
	/// Очередь файлов для записи.
	std::queue<ptr<File> > bufferedFiles;
	/// Готов ли поток к приёму данных.
	bool ready;

	/// Обработать очередь.
	void ProcessQueue();

protected:
	/// Записать данные в поток.
	/** Производный класс должен перегружать данный метод, и в нём выполнять собственно запись. */
	virtual void WriteData(const void* data, size_t size) = 0;

	/// Сообщить, что поток готов к приёму данных.
	/** Этот метод вызывается только производным классом. */
	void HandleReady();

	/// Сообщить, что произошла ошибка.
	void HandleException(ptr<Exception> exception);

public:
	AsyncOutputStream(ptr<Handler> handler);

	/// Выполнить запись данных.
	/** Метод может вызываться в любое время.
	 * Он просто добавляет отправляемые данные в очередь. */
	void Write(ptr<File> file);
};

END_INANITY

#endif
