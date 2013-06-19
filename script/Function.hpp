#ifndef ___INANITY_SCRIPT_FUNCTION_HPP___
#define ___INANITY_SCRIPT_FUNCTION_HPP___

#include "script.hpp"

BEGIN_INANITY_SCRIPT

/// Abstract class represents function from script.
class Function : public Object
{
public:
	/// Run the function.
	virtual void Run() = 0;
};

END_INANITY_SCRIPT

#endif
