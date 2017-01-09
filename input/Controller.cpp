#include "Controller.hpp"

BEGIN_INANITY_INPUT

Controller::Controller(int controllerId)
: controllerId(controllerId) {}

int Controller::GetControllerId() const
{
	return controllerId;
}

END_INANITY_INPUT
