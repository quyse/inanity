#include "DllCache.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

#ifdef ___INANITY_PLATFORM_WINDOWS

DllCache::Dlls DllCache::dlls;

HMODULE DllCache::Load(const String& name)
{

	Dlls::const_iterator i = dlls.find(name);
	HMODULE dll = 0;
	if(i == dlls.end())
	{
		dll = LoadLibrary(Strings::UTF82Unicode(name).c_str());
		dlls.insert(std::make_pair(name, dll));
	}
	else
		dll = i->second;

	if(!dll)
		THROW("Can't load dll " + name);

	return dll;

}

#endif

END_INANITY_PLATFORM
