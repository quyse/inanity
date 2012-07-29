#ifndef ___INANITY_CONFIG_HPP___
#define ___INANITY_CONFIG_HPP___

/*
 * Конфигурация Inanity для компиляции.
 */

#if defined(_WIN32) || defined(__WIN32__)
#define ___INANITY_WINDOWS
#else
#define ___INANITY_LINUX
#endif

#ifdef ___INANITY_LINUX
#define DebugBreak() asm("int3")
#endif // ___INANITY_LINUX

//#define ___INANITY_SCRIPTING

/// Тип для "больших размеров" данных.
/** То есть для таких, которые, возможно, не помещаются
в память целиком, но мы хотим их обрабатывать. */
typedef unsigned long long bigsize_t;

#define BEGIN_INANITY namespace Inanity {
#define END_INANITY }

#ifdef INANITY_LIB

BEGIN_INANITY
END_INANITY
using namespace Inanity;

#endif

BEGIN_INANITY

/// "Быстрое" преобразование типа.
/** В отладке это dynamic_cast, в релизе - static_cast. */
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

#endif
