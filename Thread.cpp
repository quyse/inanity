#include "Thread.hpp"
#include "Exception.hpp"
#ifdef ___INANITY_LINUX
#include <pthread.h>
#endif

Thread::Thread(ptr<Handler> handler)
{
	try
	{
		this->handler = handler;

#ifdef ___INANITY_LINUX
		if(pthread_create(&thread, 0, ThreadRoutine, this))
			THROW_SECONDARY_EXCEPTION("pthread_create failed", Exception::SystemError());
#endif
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create thread", exception);
	}
}

Thread::~Thread()
{
}

void* Thread::ThreadRoutine(void* self)
{
	((Thread*)self)->Run();
	return 0;
}

void Thread::Run()
{
	handler->Fire(this);
}

void Thread::WaitEnd()
{
#ifdef ___INANITY_LINUX
	if(pthread_join(thread, 0))
		THROW_SECONDARY_EXCEPTION("Can't wait for thread end", Exception::SystemError());
#endif
}
