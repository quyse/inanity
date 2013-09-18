#include "Thread.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_LINUX
#include <pthread.h>
#include <time.h>
#endif

BEGIN_INANITY

Thread::Thread(ptr<ThreadHandler> handler) : handler(handler)
{
	try
	{
		Reference();
#ifdef ___INANITY_WINDOWS
		thread = NEW(Handle(CreateThread(0, 0, ThreadRoutine, this, 0, 0)));
		if(!thread->IsValid())
		{
			Dereference();
			THROW_SECONDARY("CreateThread failed", Exception::SystemError());
		}
#endif
#ifdef ___INANITY_LINUX
		if(pthread_create(&thread, 0, ThreadRoutine, this))
		{
			Dereference();
			THROW_SECONDARY("pthread_create failed", Exception::SystemError());
		}
#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create thread", exception);
	}
}

#ifdef ___INANITY_WINDOWS
DWORD CALLBACK Thread::ThreadRoutine(void* self)
{
	((Thread*)self)->Run();
	return 0;
}
#endif
#ifdef ___INANITY_LINUX
void* Thread::ThreadRoutine(void* self)
{
	((Thread*)self)->Run();
	return 0;
}
#endif

void Thread::Run()
{
	handler->FireData(this);
	Dereference();
}

void Thread::WaitEnd()
{
#ifdef ___INANITY_WINDOWS
	if(WaitForSingleObject(*thread, INFINITE) != WAIT_OBJECT_0)
#endif
#ifdef ___INANITY_LINUX
	if(pthread_join(thread, 0))
#endif
		THROW_SECONDARY("Can't wait for thread end", Exception::SystemError());
}

void Thread::Sleep(int milliseconds)
{
#ifdef ___INANITY_WINDOWS
  ::Sleep(milliseconds);
#endif
#ifdef ___INANITY_LINUX
  struct timespec t;
  t.tv_sec = (time_t)(milliseconds / 1000);
  t.tv_nsec = (long)((milliseconds % 1000) * 1000000);
  nanosleep(&t, 0);
#endif
}

END_INANITY
