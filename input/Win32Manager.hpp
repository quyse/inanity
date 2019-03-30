#ifndef ___INANITY_INPUT_WIN32_MANAGER_HPP___
#define ___INANITY_INPUT_WIN32_MANAGER_HPP___

#include "Manager.hpp"
#include "Key.hpp"
#include "../platform/windows.hpp"

BEGIN_INANITY_INPUT

/// General class for Win32 input managers.
class Win32Manager : public Manager
{
private:
	int cursorX, cursorY;

protected:
	Win32Manager();

	static Key ConvertKey(USHORT key, USHORT makeCode, USHORT flags);

public:
	virtual bool ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

END_INANITY_INPUT

#endif
