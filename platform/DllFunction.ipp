#ifndef ___INANITY_PLATFORM_DLL_FUNCTION_IPP___
#define ___INANITY_PLATFORM_DLL_FUNCTION_IPP___

#include "DllFunction.hpp"
#include "DllCache.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

template <typename Function>
DllFunction<Function>::DllFunction(const String& dllName, const char* functionName)
{
	BEGIN_TRY();

#ifdef ___INANITY_PLATFORM_WINDOWS
	HMODULE hModule = DllCache::Load(dllName);
	void* f = GetProcAddress(hModule, functionName);
	if(!f)
		THROW("Function was not found");
	function = (Function)f;
#endif

	END_TRY(String("Can't initialize DLL function ") + functionName + " from " + dllName);
}

template <typename Function>
DllFunction<Function>::operator Function() const
{
	return function;
}

END_INANITY_PLATFORM

#endif
