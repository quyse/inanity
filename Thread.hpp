#ifndef ___INANITY_THREAD_HPP___
#define ___INANITY_THREAD_HPP___

#include "Handler.hpp"
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
	typedef DataHandler<ptr<Thread> > ThreadHandler;

private:
	ptr<ThreadHandler> handler;

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
	Thread(ptr<ThreadHandler> handler);

	void WaitEnd();

	static void Sleep(int milliseconds);
};

END_INANITY

#endif
