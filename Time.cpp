#include "Time.hpp"
#ifdef ___INANITY_WINDOWS
#include "windows.hpp"
#endif
#include <ctime>

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
	// FIXME: неточное измерение
	return GetUnixTime();
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
	// FIXME: неточное измерение
	return 1;
#endif
}
