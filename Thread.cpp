#include "Thread.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_PLATFORM_POSIX
#include <pthread.h>
#include <time.h>
#endif

BEGIN_INANITY

Thread::Thread(ptr<ThreadHandler> handler) : handler(handler)
{
	try
	{
#if defined(___INANITY_PLATFORM_WINDOWS)
		thread = NEW(Platform::Win32Handle(CreateThread(0, 0, ThreadRoutine, this, 0, 0)));
		if(!thread->IsValid())
			THROW_SECONDARY("CreateThread failed", Exception::SystemError());
#elif defined(___INANITY_PLATFORM_POSIX)
		if(pthread_create(&thread, 0, ThreadRoutine, this))
			THROW_SECONDARY("pthread_create failed", Exception::SystemError());
#else
#error Unknown platform
#endif
		Reference();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create thread", exception);
	}
}

#if defined(___INANITY_PLATFORM_WINDOWS)
DWORD CALLBACK Thread::ThreadRoutine(void* self)
{
	((Thread*)self)->Run();
	return 0;
}
#elif defined(___INANITY_PLATFORM_POSIX)
void* Thread::ThreadRoutine(void* self)
{
	((Thread*)self)->Run();
	return 0;
}
#else
#error Unknown platform
#endif

void Thread::Run()
{
	handler->FireData(this);
	Dereference();
}

void Thread::WaitEnd()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	if(WaitForSingleObject(*thread, INFINITE) != WAIT_OBJECT_0)
#elif defined(___INANITY_PLATFORM_POSIX)
	if(pthread_join(thread, 0))
#else
#error Unknown platform
#endif
		THROW_SECONDARY("Can't wait for thread end", Exception::SystemError());
}

void Thread::Sleep(int milliseconds)
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	::Sleep(milliseconds);
#elif defined(___INANITY_PLATFORM_POSIX)
	struct timespec t;
	t.tv_sec = (time_t)(milliseconds / 1000);
	t.tv_nsec = (long)((milliseconds % 1000) * 1000000);
	nanosleep(&t, 0);
#else
#error Unknown platform
#endif
}

END_INANITY
