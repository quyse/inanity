#ifndef ___INANITY_SCRIPT_FUNCTION_HPP___
#define ___INANITY_SCRIPT_FUNCTION_HPP___

#include "script.hpp"

BEGIN_INANITY_SCRIPT

class Any;

/// Abstract class represents function from script.
class Function : public Object
{
public:
	/// Run the function.
	virtual ptr<Any> Run() = 0;
};

END_INANITY_SCRIPT

#endif
