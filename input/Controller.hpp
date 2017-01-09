#ifndef ___INANITY_INPUT_CONTROLLER_HPP___
#define ___INANITY_INPUT_CONTROLLER_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

/// Abstract game controller.
class Controller : public Object
{
protected:
	/// Controller id to distinguish events for this controller.
	int controllerId;

	Controller(int controllerId);

public:
	/// Get controller id.
	int GetControllerId() const;

	/// Return if controller is still connected.
	virtual bool IsActive() const = 0;
};

END_INANITY_INPUT

#endif
