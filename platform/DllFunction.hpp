#ifndef ___INANITY_PLATFORM_DLL_FUNCTION_HPP___
#define ___INANITY_PLATFORM_DLL_FUNCTION_HPP___

#include "platform.hpp"
#include "../String.hpp"

BEGIN_INANITY_PLATFORM

/// Class of a function got from dynamic link library.
template <typename Function>
class DllFunction
{
private:
	Function function;

public:
	DllFunction(const String& dllName, const char* functionName);

	operator Function() const;
};

END_INANITY_PLATFORM

#endif
