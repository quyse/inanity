#ifndef ___INANITY_META_METHOD_IPP___
#define ___INANITY_META_METHOD_IPP___

#include "Method.hpp"
#include "Callable.ipp"

BEGIN_INANITY_META

#ifdef ___INANITY_META_LUA___
template <typename MethodType, MethodType method>
Script::Lua::MethodExtensionBase* Method<MethodType, method>::GetLuaExtension()
{
	return &luaExtension;
}
#endif

template <typename MethodType, MethodType method>
Method<MethodType, method>::Method(const char* name) : MethodBase(name)
{
	// register the method in the class
	typedef typename Callable<MethodType>::ClassType ClassType;
	ClassType::meta.AddMethod(this);
}

END_INANITY_META

#endif
