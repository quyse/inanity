#ifndef ___INANITY_INPUT_CORE_MANAGER_HPP___
#define ___INANITY_INPUT_CORE_MANAGER_HPP___

#include "Manager.hpp"
#include "Key.hpp"
#include "../platform/platform.hpp"
#include <agile.h>

BEGIN_INANITY_PLATFORM

class CoreWindow;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

/// UWP/Xbox input manager.
class CoreManager : public Manager
{
private:
	ref class Impl;
	::Platform::Agile<Impl> impl;

public:
	CoreManager(ptr<Platform::CoreWindow> window);
	~CoreManager();

	// Manager's methods.
	void Update();
	ptr<Controller> TryGetController(uint64_t controllerId);
};

END_INANITY_INPUT

#endif
