#ifndef ___INANITY_INPUT_X11_MANAGER_HPP___
#define ___INANITY_INPUT_X11_MANAGER_HPP___

#include "Manager.hpp"
#include "../platform/x11.hpp"

BEGIN_INANITY_INPUT

class X11Manager : public Manager
{
private:
	int mouseX, mouseY;

public:
	X11Manager();

	bool Process(const XEvent& event);
};

END_INANITY_INPUT

#endif
