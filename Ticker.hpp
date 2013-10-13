#ifndef ___INANITY_TICKER_HPP___
#define ___INANITY_TICKER_HPP___

#include "Time.hpp"

BEGIN_INANITY

/// Helper class to calculate time between ticks.
class Ticker
{
private:
	/// Time length of one clock tick.
	float tickCoef;
	/// Last clock tick value.
	/** -1 if there was no tick yet. */
	Time::Tick lastTick;
	/// Pause tick value.
	/** -1 if not paused */
	Time::Tick pauseTick;

public:
	Ticker();

	/// Sets ticker on pause.
	void Pause();
	/// Marks a new tick, returns time from last tick.
	/** If ticker was paused, it resumes. */
	float Tick();
};

END_INANITY

#endif
