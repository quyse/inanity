#include "Controller.hpp"

BEGIN_INANITY_INPUT

Controller::Controller(uint64_t controllerId)
: controllerId(controllerId) {}

uint64_t Controller::GetControllerId() const
{
	return controllerId;
}

END_INANITY_INPUT
