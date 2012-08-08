#ifndef ___INANITY_PROFILING_HPP___
#define ___INANITY_PROFILING_HPP___

/* Файл, управляющий профилированием движка.
Профилирование - это измерение временных характеристик
выполнения различных функций.

Пока профилирование поддерживает только один поток выполнения.
*/

#ifdef _DEBUG
// Включено ли профилирование.
#define ___INANITY_PROFILING
#endif

#ifdef ___INANITY_PROFILING

#include <ostream>
// Необходимо для измерений времени.
#include "Time.hpp"
// Необходимо для макроса __SLINE__ :(
#include "Exception.hpp"

// ******* Настройки профилирования.

/// Максимальное количество записей профилирования.
#define PROFILE_MAXIMUM_RECORDS_COUNT 0x1000000



// ******* Макросы, используемые в коде.

/// Макрос, описывающий позицию в коде.
#define PROFILE_CODE_POSITION \
	__FUNCTION__ "(" __FILE__ ":" __SLINE__ ")"

/// Макрос, профилирующий определённую точку.
#define PROFILE_POINT() \
	Inanity::Profiling::Record(Inanity::Profiling::recordTypePoint, PROFILE_CODE_POSITION)

/// Макрос, профилирующий область видимости.
#define PROFILE_SCOPE() \
	Inanity::Profiling::Scope profileScope(PROFILE_CODE_POSITION)



// ******* Реализация профилирования.

/// Пространство имён с функциями профилирования.
/** Так как профилирование - отладочная фича, то незачем делать объектно-ориентированный профайлер,
или типа того. Напротив, так как профилирование должно оказывать минимальное влияние на программу,
всё должно быть как можно проще и легче. */

BEGIN_INANITY

namespace Profiling
{
	/// Типы профилирующих записей.
	enum RecordType
	{
		/// Старт профилирования.
		recordTypeStart,
		/// Стоп профилирования.
		recordTypeStop,
		/// Точка профилирования.
		recordTypePoint,
		/// Вход в область видимости.
		recordTypeScopeEnter,
		/// Выход из области видимости.
		recordTypeScopeLeave
	};

	/// Включено ли профилирование.
	extern bool profiling;

	// Для большей эффективности записи хранятся не структурами, а массивами.
	/// Времена записей.
	extern long long recordTimes[PROFILE_MAXIMUM_RECORDS_COUNT];
	/// Типы записей.
	extern RecordType recordTypes[PROFILE_MAXIMUM_RECORDS_COUNT];
	/// Позиции записей.
	extern const char* recordPositions[PROFILE_MAXIMUM_RECORDS_COUNT];
	/// Текущее количество записей.
	extern size_t recordsCount;

	/// Сделать запись.
	inline void Record(RecordType recordType, const char* position)
	{
		if(profiling && recordsCount < PROFILE_MAXIMUM_RECORDS_COUNT)
		{
			recordTimes[recordsCount] = Inanity::Time::GetTicks();
			recordTypes[recordsCount] = recordType;
			recordPositions[recordsCount] = position;
			++recordsCount;
		}
	}

	/// Начать профилирование.
	void Start();

	/// Завершить профилирование.
	/** То есть, поставить на паузу. */
	void Stop();

	/// Сбросить собранную статистику.
	/** Можно вызывать и при работающем профилировании. */
	void Reset();

	/// Сформировать красивый отчёт :)
	void Report(std::ostream& stream);

	/// Класс, выполняющий записи о входе-выходе в область видимости.
	class Scope
	{
	private:
		/// Сохранённая позиция.
		const char* position;

	public:
		inline Scope(const char* position) : position(position)
		{
			Record(recordTypeScopeEnter, position);
		}
		inline ~Scope()
		{
			Record(recordTypeScopeLeave, position);
		}
	};
}

END_INANITY

#else

// Заглушки для макросов при отключенном профилировании.

#define PROFILE_POINT()
#define PROFILE_SCOPE()

#endif

#endif
