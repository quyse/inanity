#ifndef ___INANITY_FUTURE_HPP___
#define ___INANITY_FUTURE_HPP___

#include "HandlerQueue.hpp"
#include "CriticalSection.hpp"
#include "CriticalCode.hpp"
#include "Exception.hpp"

BEGIN_INANITY

/// Класс, представляющий результат асинхронной работы.
/** Предоставляется результат и объект исключения, если произошла ошибка.
Всегда нужно сначала проверять объект исключения (== null).
Этот механизм - асинхронная замена бросанию исключений через throw. */
template <typename T>
class Future : public Object
{
public:
	/// Тип обрабочика.
	typedef Handler2<T, ptr<Exception> > Target;

private:
	/// Критическая секция.
	CriticalSection criticalSection;
	/// Очередь для выполнения.
	ptr<HandlerQueue> queue;
	/// Список функций для вызова.
	std::vector<ptr<Target> > targets;
	/// Была ли завершёна работа?
	bool finished;
	/// Результат работы.
	T result;
	/// Исключение в случае ошибки.
	ptr<Exception> exception;

	/// Специальный класс-делегат для отложенного вызова обработчиков.
	class Delegate : public VoidHandler
	{
	private:
		ptr<Future> future;
		ptr<Target> target;

	public:
		Delegate(ptr<Future> future, ptr<Target> target)
		: future(future), target(target) {}

	protected:
		void OnEvent()
		{
			target->Fire(future->result, future->exception);
		}
	};

private:
	/// Выполнить обработчик.
	/** Можно вызывать, только если finished == true. */
	void Run(ptr<Target> target)
	{
		// если очередь задана, положить в неё
		if(queue)
			queue->Enqueue(NEW(Delegate(this, target)));
		// иначе просто выполнить
		else
			target->Fire(result, exception);
	}

	/// Выполнить обработчики.
	void Finish()
	{
		// установить флажок завершённости, и забрать список обработчиков
		std::vector<ptr<Target> > targets;
		{
			CriticalCode code(criticalSection);
			finished = true;
			targets.swap(this->targets);
		}

		// с этого момента добавляемые обработчики будут сразу выполняться

		// вызвать полученные обработчики
		for(size_t i = 0; i < targets.size(); ++i)
			Run(targets[i]);
	}

public:
	Future(ptr<HandlerQueue> queue = 0)
	: queue(queue), finished(false) {}

	/// Добавить обработчик завершения.
	void AddTarget(ptr<Target> target)
	{
		{
			CriticalCode code(criticalSection);
			// если работа ещё не выполнена
			if(!finished)
			{
				// добавить обработчик в список
				targets.push_back(target);
				return;
			}
		}
		// иначе работа уже выполнена
		// просто запустить обработчик
		Run(target);
	}

	/// Указать, что работа успешно завершена.
	void Result(T result)
	{
		this->result = result;
		Finish();
	}

	/// Указать, что произошла ошибка.
	void Error(ptr<Exception> exception)
	{
		this->exception = exception;
		Finish();
	}
};

END_INANITY

#endif
