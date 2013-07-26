#ifndef ___INANITY_PLATFORM_DLL_CACHE_HPP___
#define ___INANITY_PLATFORM_DLL_CACHE_HPP___

#include "platform.hpp"
#ifdef ___INANITY_WINDOWS
#include <unordered_map>
#include "../String.hpp"
#include "../windows.hpp"
#endif

BEGIN_INANITY_PLATFORM

/// Cache of dynamic-link libraries.
class DllCache
{
#ifdef ___INANITY_WINDOWS

private:
	typedef std::unordered_map<String, HMODULE> Dlls;
	static Dlls dlls;

public:
	static HMODULE Load(const String& name);

#endif
};

END_INANITY_PLATFORM

#endif
