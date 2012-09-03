#ifndef ___INANITY_HANDLER_QUEUE_HPP___
#define ___INANITY_HANDLER_QUEUE_HPP___

#include "Handler.hpp"
#include "CriticalSection.hpp"
#include "Semaphore.hpp"
#include <queue>

BEGIN_INANITY

/// Класс очереди на обработку.
/** Очередь получает элементы на обработку, и выполняет их по порядку.
Класс поддерживает любое количество потоков обработки. */
class HandlerQueue : public Object
{
private:
	/// Очередь на обработку.
	std::queue<ptr<VoidHandler> > handlers;
	/// Критическая секция для доступа к очереди.
	CriticalSection criticalSection;
	/// Семафор для очереди.
	Semaphore semaphore;
	/// Флажок завершения работы.
	volatile bool stop;

	/// Вспомогательный обработчик, выполняющий заданный ему обработчик асинхронно.
	class EnqueueHandler : public VoidHandler
	{
	private:
		ptr<VoidHandler> handler;

	public:
		EnqueueHandler(ptr<VoidHandler> handler);
		void OnEvent();
	};

public:
	HandlerQueue();

	/// Добавить обработчик в очередь.
	/** Можно вызывать из любого потока. */
	void Enqueue(ptr<VoidHandler> handler);

	/// Выполнять очередь бесконечно.
	/** Можно вызывать из разных потоков, при этом потоки
	будут произвольно разделять нагрузку между собой.
	Остановить выполнение можно только вызовом Finish. */
	void Run();

	/// Остановить очередь.
	/** Выполняется мгновенно. Потоки, выполняющие обработчики,
	выполняют всё, что есть в очереди, и завершаются. */
	void Finish();

	static ptr<VoidHandler> CreateEnqueueHandler(ptr<VoidHandler> handler);
};

END_INANITY

#endif
