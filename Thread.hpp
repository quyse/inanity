#ifndef ___INANITY_THREAD_HPP___
#define ___INANITY_THREAD_HPP___

#include "EventHandler.hpp"
#ifdef ___INANITY_WINDOWS
#include "Handle.hpp"
#include "windows.hpp"
#endif
#ifdef ___INANITY_LINUX
#include <sys/types.h>
#endif

BEGIN_INANITY

/// Класс потока выполнения.
class Thread : public Object
{
public:
	typedef EventHandler<ptr<Thread> > Handler;

private:
	ptr<Handler> handler;

#ifdef ___INANITY_WINDOWS

	ptr<Handle> thread;

	static DWORD CALLBACK ThreadRoutine(void* self);

#endif

#ifdef ___INANITY_LINUX

	pthread_t thread;

	static void* ThreadRoutine(void* self);

#endif

	void Run();

public:
	Thread(ptr<Handler> handler);

	void WaitEnd();
};

END_INANITY

#endif
