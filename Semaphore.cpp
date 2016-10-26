#include "Semaphore.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_PLATFORM_POSIX
#include <errno.h>
#endif

BEGIN_INANITY

Semaphore::Semaphore(int initialCount)
{
	try
	{
#if defined(___INANITY_PLATFORM_WINDOWS)
		handle = CreateSemaphore(NULL, initialCount, 0x7fffffff, NULL);
		if(!handle.IsValid())
#elif defined(___INANITY_PLATFORM_MACOS)
		sem = dispatch_semaphore_create(initialCount);
		if(!sem)
#elif defined(___INANITY_PLATFORM_POSIX)
		if(sem_init(&sem, 0, 0) != 0)
#else
#error Unknown platform
#endif
			THROW_SECONDARY("Can't initialize semaphore", Exception::SystemError());
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create semaphore", exception);
	}
}

Semaphore::~Semaphore()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	CloseHandle(handle);
#elif defined(___INANITY_PLATFORM_MACOS)
	dispatch_release(sem);
#elif defined(___INANITY_PLATFORM_POSIX)
	sem_destroy(&sem);
#endif
}

void Semaphore::Acquire()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	if(WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
#elif defined(___INANITY_PLATFORM_MACOS)
	if(dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER) != 0)
#elif defined(___INANITY_PLATFORM_POSIX)
	if(sem_wait(&sem) != 0)
#else
#error Unknown platform
#endif
		THROW_SECONDARY("Can't acquire semaphore", Exception::SystemError());
}

bool Semaphore::TryAcquire()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	switch(WaitForSingleObject(handle, 0))
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		return false;
	}
#elif defined(___INANITY_PLATFORM_MACOS)
	return (dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER) == 0);
#elif defined(___INANITY_PLATFORM_POSIX)
	if(sem_trywait(&sem) == 0)
		return true;
	if(errno == EAGAIN)
		return false;
#else
#error Unknown platform
#endif

	THROW_SECONDARY("Error trying acquire semaphore", Exception::SystemError());
}

void Semaphore::Release(int count)
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	if(!ReleaseSemaphore(handle, count, NULL))
#elif defined(___INANITY_PLATFORM_MACOS)
	if(dispatch_semaphore_signal(sem), 0)
#elif defined(___INANITY_PLATFORM_POSIX)
	int i;
	for(i = 0; i < count && sem_post(&sem) == 0; ++i);
	if(i < count)
#else
#error Unknown platform
#endif
		THROW_SECONDARY("Can't release semaphore", Exception::SystemError());
}

END_INANITY
