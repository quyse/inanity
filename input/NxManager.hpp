#ifndef ___INANITY_INPUT_NX_MANAGER_HPP___
#define ___INANITY_INPUT_NX_MANAGER_HPP___

#include "Manager.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class NxWindow;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

class NxManager : public Manager
{
public:
	NxManager(ptr<Platform::NxWindow> window);
};

END_INANITY_INPUT

#endif
