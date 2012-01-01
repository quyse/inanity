#include "Semaphore.hpp"
#include "Exception.hpp"

Semaphore::Semaphore(int initialCount)
{
	try
	{
#ifdef ___INANITY_WINDOWS
		handle = CreateSemaphore(NULL, initialCount, 0x7fffffff, NULL);
		if(!handle.IsValid())
			THROW_PRIMARY_EXCEPTION("Can't create semaphore handle");
#endif
#ifdef ___INANITY_LINUX
#error Dont implemented yet
#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create semaphore", exception);
	}
}

void Semaphore::Acquire()
{
#ifdef ___INANITY_WINDOWS
	if(WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
#endif
#ifdef ___INANITY_LINUX
#error Dont implemented yet
#endif
		THROW_PRIMARY_EXCEPTION("Error when acquiring semaphore");
}

void Semaphore::Release(int count)
{
#ifdef ___INANITY_WINDOWS
	if(!ReleaseSemaphore(handle, count, NULL))
#endif
#ifdef ___INANITY_LINUX
#error Dont implemented yet
#endif
		THROW_PRIMARY_EXCEPTION("Error when releasing semaphore");
}
