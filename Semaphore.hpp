#ifndef ___INANITY_SEMAPHORE_H___
#define ___INANITY_SEMAPHORE_H___

#include "Object.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "platform/windows.hpp"
#include "platform/Win32Handle.hpp"
#elif defined(___INANITY_PLATFORM_POSIX)
#include <semaphore.h>
#else
#error Unknown platform
#endif

BEGIN_INANITY

/// Класс семафора.
class Semaphore
{
private:
#if defined(___INANITY_PLATFORM_WINDOWS)
	Platform::Win32Handle handle;
#elif defined(___INANITY_PLATFORM_POSIX)
	sem_t sem;
#else
#error Unknown platform
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
