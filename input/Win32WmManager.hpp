#ifndef ___INANITY_INPUT_WIN32_WM_MANAGER_HPP___
#define ___INANITY_INPUT_WIN32_WM_MANAGER_HPP___

#include "Win32Manager.hpp"

BEGIN_INANITY_INPUT

/// Win32 input manager which all events
/// receives via usual Windows Messages.
/** Doesn't emit mouse raw move events, except for wheel. */
class Win32WmManager : public Win32Manager
{
public:
	bool ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

END_INANITY_INPUT

#endif
