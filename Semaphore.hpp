#ifndef ___INANITY_SEMAPHORE_H___
#define ___INANITY_SEMAPHORE_H___

#include "Object.hpp"
#ifdef ___INANITY_WINDOWS
#include "platform/windows.hpp"
#include "platform/Win32Handle.hpp"
#endif
#ifdef ___INANITY_LINUX
//#include <sys/types.h>
#include <semaphore.h>
#endif

BEGIN_INANITY

/// Класс семафора.
class Semaphore
{
private:
#ifdef ___INANITY_WINDOWS
	Platform::Win32Handle handle;
#endif
#ifdef ___INANITY_LINUX
	sem_t sem;
#endif

public:
	/// Создать семафор.
	Semaphore(int initialCount = 0);
	~Semaphore();

	/// Уменьшить счётчик семафора на 1, дождавшись, когда он станет больше нуля.
	void Acquire();
	/// Попытаться уменьшить счётчик семафора на 1. Не ждать, если нельзя.
	/** \returns true, если счётчик семафора уменьшен на 1. */
	bool TryAcquire();
	/// Увеличить счётчик семафора на заданное число.
	void Release(int count = 1);
};

END_INANITY

#endif
