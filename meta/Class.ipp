#ifndef ___INANITY_META_CLASS_IPP___
#define ___INANITY_META_CLASS_IPP___

#include "Class.hpp"

BEGIN_INANITY_META

#ifdef ___INANITY_META_LUA___
template <typename ClassType>
Script::Lua::ClassExtensionBase* Class<ClassType>::GetLuaExtension()
{
	return &luaExtension;
}
#endif

template <typename ClassType>
Class<ClassType>::Class(const char* name, const char* fullName)
: ClassBase(name, fullName) {}

END_INANITY_META

#endif
