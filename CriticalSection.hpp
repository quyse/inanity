#ifndef ___INANITY_CRITICAL_SECTION_H___
#define ___INANITY_CRITICAL_SECTION_H___

#include "Object.hpp"
#if defined(___INANITY_PLATFORM_WINDOWS)
#include "platform/windows.hpp"
#elif defined(___INANITY_PLATFORM_POSIX)
#include <sys/types.h>
#else
#error Unknown platform
#endif

BEGIN_INANITY

/// Класс критической секции.
class CriticalSection
{
private:
#if defined(___INANITY_PLATFORM_WINDOWS)
	CRITICAL_SECTION criticalSection;
#elif defined(___INANITY_PLATFORM_POSIX)
	pthread_mutex_t mutex;
#else
#error Unknown platform
#endif

public:
	CriticalSection();
	~CriticalSection();

	void Enter();
	void Leave();
};

END_INANITY

#endif
