#ifndef ___INANITY_INPUT_X11_MANAGER_HPP___
#define ___INANITY_INPUT_X11_MANAGER_HPP___

#include "Manager.hpp"
#include "Key.hpp"
#include "../platform/platform.hpp"
#include "../platform/x11.hpp"

BEGIN_INANITY_PLATFORM

class X11Display;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

class X11Manager : public Manager
{
private:
	ptr<Platform::X11Display> display;
	int mouseX, mouseY;
	int keyboardMapping[256];

	Key ConvertKey(unsigned key);
	void UpdateKeyboardMapping(int first, int count);

public:
	X11Manager(ptr<Platform::X11Display> display);

	void Process(const XEvent& event);
};

END_INANITY_INPUT

#endif
