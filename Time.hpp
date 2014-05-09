#ifndef ___INANITY_TIME_HPP___
#define ___INANITY_TIME_HPP___

#include "Object.hpp"

BEGIN_INANITY

/// Класс, позволяющий получать текущее время, и тики.
/** Абстракция от платформы, чтобы получать время. */

class Time
{
public:
	/// Type to represent ticks.
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
	/// On emscripten 'long long' is slow.
	typedef float Tick;
#else
	typedef long long Tick;
#endif

public:
	/// Получить текущее UNIX-время (количество миллисекунд от 0 часов 1 января 1970).
	static long long GetUnixTime();

	/// Получить текущее количество тиков, с максимальной точностью.
	/** Абсолютная величина имеет неопределённое значение (не известно, что такое 0),
	полезна только разница между двумя измерениями. */
	static Tick GetTick();
	/// Получить количество тиков в секунду.
	static Tick GetTicksPerSecond();
};

END_INANITY

#endif
