#include "HandlerQueue.hpp"
#include "CriticalCode.hpp"

BEGIN_INANITY

HandlerQueue::EnqueueHandler::EnqueueHandler(ptr<VoidHandler> handler)
: handler(handler) {}

void HandlerQueue::EnqueueHandler::OnEvent()
{
	handler->Fire();
}

HandlerQueue::HandlerQueue() : stop(false) {}

void HandlerQueue::Enqueue(ptr<VoidHandler> handler)
{
	{
		CriticalCode code(criticalSection);

		handlers.push(handler);
	}

	semaphore.Release();
}

void HandlerQueue::Run()
{
	for(;;)
	{
		// если флажок остановки выставлен
		if(stop)
		{
			// работаем, только пока есть элементы
			if(!semaphore.TryAcquire())
				break;
		}
		else
			// флажок остановки не выставлен, ждём, если нет элементов
			semaphore.Acquire();

		// элемент есть, получить его
		ptr<VoidHandler> handler;
		{
			CriticalCode code(criticalSection);
			handler = handlers.front();
		}
		// выполняем элемент
		if(handler)
			handler->Fire();
	}

	// после выхода - оставить пустой элемент в очереди
	// см. объяснение процедуры завершения ниже
	Enqueue(0);
}

/* Завершение работы происходит следующим образом.
Прежде всего, выставляется флажок завершения работы.
Потоки, которые сейчас заняты, завершат текущее задание
и гарантированно увидят установленный флажок.
Потоки, которые сейчас ожидают, флажок не увидят, потому что
они ожидают. Поэтому мы их будим, закладывая пустой элемент
в очередь.
Этот элемент, на самом деле, будит только один ожидающий поток.
Поэтому каждый поток после завершения своего цикла оставляет
после себя пустой элемент-эстафету, чтобы разбудить следующий
поток. Таким образом, в худшем случае потоки завершатся
последовательно, один за другим. Количество пустых элементов
не больше чем (количество_потоков + 1), так что ничего
страшного в них нет.
*/

void HandlerQueue::Finish()
{
	stop = true;
	Enqueue(0);
}

ptr<VoidHandler> HandlerQueue::CreateEnqueueHandler(ptr<VoidHandler> handler)
{
	return NEW(EnqueueHandler(handler));
}

END_INANITY
