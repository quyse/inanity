#ifndef ___INANITY_INPUT_MANAGER_HPP___
#define ___INANITY_INPUT_MANAGER_HPP___

#include "input.hpp"
#include "../CriticalSection.hpp"

BEGIN_INANITY_INPUT

class Frame;
struct Event;
class Controller;

/// Abstract input manager class.
class Manager : public Object
{
protected:
	/// Critical section for internal use.
	/** Only for emitting events from another thread.
	Consuming events by client should not be guarded. */
	CriticalSection criticalSection;

	/// The current input frame.
	/** Contains events for the last completed frame. */
	ptr<Frame> currentFrame;
	/// Current internal input frame.
	/** Contains latest events, but it is in-progress.
	Eventually it will become current frame. */
	ptr<Frame> internalFrame;

protected:
	Manager();

	void AddEvent(const Event& e);

public:
	/// Go to the next frame.
	virtual void Update();
	/// Gets current input frame.
	ptr<Frame> GetCurrentFrame();

	/// Get new controller by controller id.
	virtual ptr<Controller> TryGetController(int controllerId);
};

END_INANITY_INPUT

#endif
