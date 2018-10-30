#ifndef ___INANITY_INPUT_CONTROLLER_HPP___
#define ___INANITY_INPUT_CONTROLLER_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

/// Abstract game controller.
class Controller : public Object
{
protected:
	/// Controller id to distinguish events for this controller.
	uint64_t controllerId;

	Controller(uint64_t controllerId);

public:
	/// Get controller id.
	uint64_t GetControllerId() const;

	/// Return if controller is still connected.
	virtual bool IsActive() const = 0;
	virtual void RunHapticLeftRight(float left, float right) = 0;
	virtual void StopHaptic() = 0;
};

END_INANITY_INPUT

#endif
