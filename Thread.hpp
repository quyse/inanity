#ifndef ___INANITY_THREAD_HPP___
#define ___INANITY_THREAD_HPP___

#include "Handler.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "platform/Win32Handle.hpp"
#include "platform/windows.hpp"
#elif defined(___INANITY_PLATFORM_POSIX)
#include <sys/types.h>
#else
#error Unknown platform
#endif

BEGIN_INANITY

/// Класс потока выполнения.
class Thread : public Object
{
public:
	typedef DataHandler<ptr<Thread> > ThreadHandler;

private:
	ptr<ThreadHandler> handler;

#if defined(___INANITY_PLATFORM_WINDOWS)

	ptr<Platform::Win32Handle> thread;

	static DWORD CALLBACK ThreadRoutine(void* self);

#elif defined(___INANITY_PLATFORM_POSIX)

	pthread_t thread;

	static void* ThreadRoutine(void* self);

#else

#error Unknown platform

#endif

	void Run();

public:
	Thread(ptr<ThreadHandler> handler);

	void WaitEnd();

	static void Sleep(int milliseconds);
};

END_INANITY

#endif
