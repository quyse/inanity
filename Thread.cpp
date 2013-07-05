#include "Thread.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_LINUX
#include <pthread.h>
#endif

BEGIN_INANITY

Thread::Thread(ptr<ThreadHandler> handler) : handler(handler)
{
	try
	{
#ifdef ___INANITY_WINDOWS
		thread = NEW(Handle(CreateThread(0, 0, ThreadRoutine, this, 0, 0)));
		if(!thread->IsValid())
			THROW_SECONDARY_EXCEPTION("CreateThread failed", Exception::SystemError());
#endif
#ifdef ___INANITY_LINUX
		if(pthread_create(&thread, 0, ThreadRoutine, this))
			THROW_SECONDARY_EXCEPTION("pthread_create failed", Exception::SystemError());
#endif
		Reference();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create thread", exception);
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
		THROW_SECONDARY_EXCEPTION("Can't wait for thread end", Exception::SystemError());
}

END_INANITY
