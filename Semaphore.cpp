#include "Semaphore.hpp"
#include "Exception.hpp"

BEGIN_INANITY

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
			THROW_SECONDARY("Can't initialize semaphore", Exception::SystemError());
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create semaphore", exception);
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
		THROW_SECONDARY("Can't acquire semaphore", Exception::SystemError());
}

bool Semaphore::TryAcquire()
{
#ifdef ___INANITY_WINDOWS
	switch(WaitForSingleObject(handle, 0))
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		return false;
	}
#endif

#ifdef ___INANITY_LINUX
	if(sem_trywait(&sem) == 0)
		return true;
	if(errno == EAGAIN)
		return false;
#endif

	THROW_SECONDARY("Error trying acquire semaphore", Exception::SystemError());
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
		THROW_SECONDARY("Can't release semaphore", Exception::SystemError());
}

END_INANITY
