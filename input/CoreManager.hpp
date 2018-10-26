#ifndef ___INANITY_INPUT_CORE_MANAGER_HPP___
#define ___INANITY_INPUT_CORE_MANAGER_HPP___

#include "Manager.hpp"
#include "Key.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class CoreWindow;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

/// UWP/Xbox input manager.
class CoreManager : public Manager
{
public:
	CoreManager(ptr<Platform::CoreWindow> window);
};

END_INANITY_INPUT

#endif
