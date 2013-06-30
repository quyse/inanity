#ifndef ___INANITY_META_CONSTRUCTOR_IPP___
#define ___INANITY_META_CONSTRUCTOR_IPP___

#include "Constructor.hpp"
#include "Callable.hpp"

BEGIN_INANITY_META

#ifdef ___INANITY_META_LUA___
template <typename ConstructorType>
Script::Lua::ConstructorExtensionBase* Constructor<ConstructorType>::GetLuaExtension()
{
	return &luaExtension;
}
#endif

END_INANITY_META

#endif
