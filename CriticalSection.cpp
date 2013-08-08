#include "CriticalSection.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_LINUX
#include <pthread.h>
#endif

BEGIN_INANITY

CriticalSection::CriticalSection()
{
	try
	{
	#ifdef ___INANITY_WINDOWS
		InitializeCriticalSection(&criticalSection);
	#endif
	#ifdef ___INANITY_LINUX
		if(pthread_mutex_init(&mutex, 0))
			THROW_SECONDARY("Can't initialize mutex", Exception::SystemError());
	#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create critical section", exception);
	}
}

CriticalSection::~CriticalSection()
{
#ifdef ___INANITY_WINDOWS
	DeleteCriticalSection(&criticalSection);
#endif
#ifdef ___INANITY_LINUX
	pthread_mutex_destroy(&mutex);
#endif
}

void CriticalSection::Enter()
{
#ifdef ___INANITY_WINDOWS
	EnterCriticalSection(&criticalSection);
#endif
#ifdef ___INANITY_LINUX
	if(pthread_mutex_lock(&mutex))
		THROW_SECONDARY("Can't enter critical section", Exception::SystemError());
#endif
}

void CriticalSection::Leave()
{
#ifdef ___INANITY_WINDOWS
	LeaveCriticalSection(&criticalSection);
#endif
#ifdef ___INANITY_LINUX
	if(pthread_mutex_unlock(&mutex))
		THROW_SECONDARY("Can't leave critical section", Exception::SystemError());
#endif
}

END_INANITY
