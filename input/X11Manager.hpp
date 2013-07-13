#ifndef ___INANITY_INPUT_X11_MANAGER_HPP___
#define ___INANITY_INPUT_X11_MANAGER_HPP___

#include "Manager.hpp"

BEGIN_INANITY_INPUT

class X11Manager : public Manager
{
public:
	X11Manager();

	/// Manager's methods.
	void Update();
};

END_INANITY_INPUT

#endif
