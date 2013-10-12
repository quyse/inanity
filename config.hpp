#ifndef ___INANITY_CONFIG_HPP___
#define ___INANITY_CONFIG_HPP___

/*
 * Inanity compilation configuration.
 */

#if defined(_WIN32) || defined(__WIN32__)
#define ___INANITY_PLATFORM_WINDOWS
#elif defined(EMSCRIPTEN)
#define ___INANITY_PLATFORM_EMSCRIPTEN
#define ___INANITY_PLATFORM_POSIX
#else
#define ___INANITY_PLATFORM_LINUX
#define ___INANITY_PLATFORM_POSIX
#endif

/// "Big data size" type.
/** I.e. that amount of data maybe not fit in memory,
but we want to deal with such amounts. */
typedef unsigned long long bigsize_t;

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
