#include "Time.hpp"
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "platform/windows.hpp"
#endif
#ifdef ___INANITY_PLATFORM_MACOS
#include <mach/mach_time.h>
#endif
#ifdef ___INANITY_PLATFORM_SWITCH
#include <nn/os.h>
#endif
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif
#include <ctime>

BEGIN_INANITY

long long Time::GetUnixTime()
{
	// используем стандартную функцию C++
	return time(0);
}

#if defined(___INANITY_PLATFORM_WINDOWS)

long long Time::GetTick()
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

#elif defined(___INANITY_PLATFORM_EMSCRIPTEN)

Time::Tick Time::GetTick()
{
	return emscripten_get_now();
}

Time::Tick Time::GetTicksPerSecond()
{
	return 1000.0f;
}

#elif defined(___INANITY_PLATFORM_MACOS)

long long Time::GetTick()
{
	return mach_absolute_time();
}

long long Time::GetTicksPerSecond()
{
	mach_timebase_info_data_t timebase;
	mach_timebase_info(&timebase);
	return 1000000000LL * timebase.numer / timebase.denom;
}

#elif defined(___INANITY_PLATFORM_SWITCH)

long long Time::GetTick()
{
	return nn::os::GetSystemTick().GetInt64Value();
}

long long Time::GetTicksPerSecond()
{
	return nn::os::GetSystemTickFrequency();
}

#elif defined(___INANITY_PLATFORM_POSIX)

long long Time::GetTick()
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
