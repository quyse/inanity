#include "CriticalSection.hpp"
#include "Exception.hpp"

BEGIN_INANITY

CriticalSection::CriticalSection()
{
	try
	{
#if defined(___INANITY_PLATFORM_WINDOWS)
		InitializeCriticalSection(&criticalSection);
#elif defined(___INANITY_PLATFORM_POSIX)
		if(pthread_mutex_init(&mutex, 0))
			THROW_SECONDARY("Can't initialize mutex", Exception::SystemError());
#else
#error Unknown platform
#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create critical section", exception);
	}
}

CriticalSection::~CriticalSection()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	DeleteCriticalSection(&criticalSection);
#elif defined(___INANITY_PLATFORM_POSIX)
	pthread_mutex_destroy(&mutex);
#else
#error Unknown platform
#endif
}

void CriticalSection::Enter()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	EnterCriticalSection(&criticalSection);
#elif defined(___INANITY_PLATFORM_POSIX)
	if(pthread_mutex_lock(&mutex))
		THROW_SECONDARY("Can't enter critical section", Exception::SystemError());
#else
#error Unknown platform
#endif
}

void CriticalSection::Leave()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	LeaveCriticalSection(&criticalSection);
#elif defined(___INANITY_PLATFORM_POSIX)
	if(pthread_mutex_unlock(&mutex))
		THROW_SECONDARY("Can't leave critical section", Exception::SystemError());
#else
#error Unknown platform
#endif
}

END_INANITY
