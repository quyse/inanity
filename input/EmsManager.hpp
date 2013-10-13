#ifndef ___INANITY_INPUT_EMS_MANAGER_HPP___
#define ___INANITY_INPUT_EMS_MANAGER_HPP___

#include "Manager.hpp"

BEGIN_INANITY_INPUT

/// Input manager class for Emscripten platform.
class EmsManager : public Manager
{
public:
	EmsManager();
};

END_INANITY_INPUT

#endif
