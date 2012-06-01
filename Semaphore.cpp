#include "Semaphore.hpp"
#include "Exception.hpp"

Semaphore::Semaphore(int initialCount)
{
	try
	{
#ifdef ___INANITY_WINDOWS
		handle = CreateSemaphore(NULL, initialCount, 0x7fffffff, NULL);
		if(!handle.IsValid())
#endif
#ifdef ___INANITY_LINUX
		if(sem_init(&sem, 0, 0) != 0)
#endif
			THROW_SECONDARY_EXCEPTION("Can't initialize semaphore", Exception::SystemError());
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create semaphore", exception);
	}
}

Semaphore::~Semaphore()
{
#ifdef ___INANITY_LINUX
	sem_destroy(&sem);
#endif
}

void Semaphore::Acquire()
{
#ifdef ___INANITY_WINDOWS
	if(WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
#endif
#ifdef ___INANITY_LINUX
	if(sem_wait(&sem) != 0)
#endif
		THROW_SECONDARY_EXCEPTION("Can't acquire semaphore", Exception::SystemError());
}

void Semaphore::Release(int count)
{
#ifdef ___INANITY_WINDOWS
	if(!ReleaseSemaphore(handle, count, NULL))
#endif
#ifdef ___INANITY_LINUX
	int i;
	for(i = 0; i < count && sem_post(&sem) == 0; ++i);
	if(i < count)
#endif
		THROW_SECONDARY_EXCEPTION("Can't release semaphore", Exception::SystemError());
}
