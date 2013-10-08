#include "Time.hpp"
#ifdef ___INANITY_WINDOWS
#include "platform/windows.hpp"
#endif
#include <ctime>

BEGIN_INANITY

long long Time::GetUnixTime()
{
	// используем стандартную функцию C++
	return time(0);
}

long long Time::GetTicks()
{
#ifdef ___INANITY_WINDOWS
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
#endif
#ifdef ___INANITY_LINUX
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000000000LL + t.tv_nsec;
#endif
}

long long Time::GetTicksPerSecond()
{
#ifdef ___INANITY_WINDOWS
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	return li.QuadPart;
#endif
#ifdef ___INANITY_LINUX
	return 1000000000LL;
#endif
}

END_INANITY
