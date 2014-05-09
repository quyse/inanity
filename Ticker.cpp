#include "Ticker.hpp"

BEGIN_INANITY

Ticker::Ticker() :
	lastTick(-1),
	pauseTick(-1)
{
	tickCoef = 1.0f / Time::GetTicksPerSecond();
}

void Ticker::Pause()
{
	// if already paused, do nothing
	if(pauseTick >= 0)
		return;

	// remember pause time
	pauseTick = Time::GetTick();
}

float Ticker::Tick()
{
	// get current time
	Time::Tick currentTick = Time::GetTick();

	// calculate amount of ticks from last tick
	Time::Tick ticks = 0;
	if(lastTick >= 0)
	{
		if(pauseTick >= 0)
			ticks = pauseTick - lastTick;
		else
			ticks = currentTick - lastTick;
	}

	// if was paused, resume
	if(pauseTick >= 0)
		pauseTick = -1;

	// remember current tick as last tick
	lastTick = currentTick;

	return ticks * tickCoef;
}

END_INANITY
