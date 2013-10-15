#ifndef ___INANITY_INPUT_FRAME_HPP___
#define ___INANITY_INPUT_FRAME_HPP___

#include "Event.hpp"
#include "State.hpp"
#include <vector>

BEGIN_INANITY_INPUT

/// Input frame.
/** Provides list of events happened in a frame, and tracks current input state
while events are forwarding.

Usage:

while(frame->NextEvent())
{
	e = GetCurrentEvent();
	// parse event...
	s = GetCurrentState();
	// use current state
}
*/
class Frame : public Object
{
private:
	std::vector<Event> events;
	size_t nextEvent;
	State state;

public:
	Frame();

	const Event& GetCurrentEvent() const;
	const State& GetCurrentState() const;
	bool NextEvent();
	void ForwardEvents();

	/// Resets frame.
	/** Clears event list. */
	void Reset();
	void AddEvent(const Event& e);
	void CopyTo(ptr<Frame> frame);
};

END_INANITY_INPUT

#endif
