#ifndef ___INANITY_CRITICAL_SECTION_H___
#define ___INANITY_CRITICAL_SECTION_H___

#include "Object.hpp"
#ifdef ___INANITY_WINDOWS
#include "platform/windows.hpp"
#endif
#ifdef ___INANITY_LINUX
#include <sys/types.h>
#endif

BEGIN_INANITY

/// Класс критической секции.
class CriticalSection
{
private:
#ifdef ___INANITY_WINDOWS
	CRITICAL_SECTION criticalSection;
#endif
#ifdef ___INANITY_LINUX
	pthread_mutex_t mutex;
#endif

public:
	CriticalSection();
	~CriticalSection();

	void Enter();
	void Leave();
};

END_INANITY

#endif
