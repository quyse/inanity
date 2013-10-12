#include "Time.hpp"
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "platform/windows.hpp"
#endif
#include <ctime>

BEGIN_INANITY

long long Time::GetUnixTime()
{
	// используем стандартную функцию C++
	return time(0);
}

#if defined(___INANITY_PLATFORM_WINDOWS)

long long Time::GetTicks()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
}

long long Time::GetTicksPerSecond()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	return li.QuadPart;
}

#elif defined(___INANITY_PLATFORM_POSIX)

long long Time::GetTicks()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000000000LL + t.tv_nsec;
}

long long Time::GetTicksPerSecond()
{
	return 1000000000LL;
}

#else

#error Unknown platform

#endif

END_INANITY
