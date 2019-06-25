#ifndef ___INANITY_CONFIG_HPP___
#define ___INANITY_CONFIG_HPP___

/*
 * Inanity compilation configuration.
 */


/// Determine base platform.
#if defined(_WIN32) || defined(__WIN32__)

#define ___INANITY_PLATFORM_WINDOWS

#if defined(_XBOX_ONE)
#define ___INANITY_PLATFORM_XBOX
#define ___INANITY_PLATFORM_XBOX_ONE
#endif

#elif defined(__EMSCRIPTEN__)

#define ___INANITY_PLATFORM_EMSCRIPTEN
#define ___INANITY_PLATFORM_POSIX

#elif defined(__FreeBSD__)

#define ___INANITY_PLATFORM_FREEBSD
#define ___INANITY_PLATFORM_POSIX

#elif defined(__APPLE__) && defined(__MACH__)

#define ___INANITY_PLATFORM_MACOS
#define ___INANITY_PLATFORM_POSIX

#elif defined(__ANDROID__)

#define ___INANITY_PLATFORM_ANDROID
#define ___INANITY_PLATFORM_POSIX

#else

#define ___INANITY_PLATFORM_LINUX
#define ___INANITY_PLATFORM_POSIX

#endif


/// Desktop platform?
#if \
	(defined(___INANITY_PLATFORM_WINDOWS) && !defined(___INANITY_PLATFORM_XBOX)) || \
	defined(___INANITY_PLATFORM_LINUX) || \
	defined(___INANITY_PLATFORM_MACOS)

#define ___INANITY_PLATFORM_DESKTOP
#define ___INANITY_PLATFORM_STEAM

#endif


/// "Big data size" type.
/** I.e. that amount of data maybe not fit in memory,
but we want to deal with such amounts. */
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
// On Emscripten long long is slow.
typedef unsigned int bigsize_t;
#else
typedef unsigned long long bigsize_t;
#define ___INANITY_BIGSIZE_IS_BIG
#endif

#define BEGIN_INANITY namespace Inanity {
#define END_INANITY }

BEGIN_INANITY

/// "Fast" type conversion.
/** dynamic_cast in debug, static_cast in release. */
template <typename ToType, typename FromType>
ToType fast_cast(FromType object)
{
#ifdef _DEBUG
	return dynamic_cast<ToType>(object);
#else
	return static_cast<ToType>(object);
#endif
}

END_INANITY

//*** Debug checks.
#ifdef _DEBUG

/// Heap tracing enabled.
#define ___INANITY_TRACE_HEAP
/// Managed pointers tracing enabled.
/** Requires enable ___INANITY_TRACE_HEAP. */
#define ___INANITY_TRACE_PTR

#endif

#endif
