#ifndef ___INANITY_META_FUNCTION_IPP___
#define ___INANITY_META_FUNCTION_IPP___

#include "Function.hpp"

BEGIN_INANITY_META

#ifdef ___INANITY_META_LUA___
template <typename FunctionType, FunctionType function>
Script::Lua::FunctionExtensionBase* Function<FunctionType, function>::GetLuaExtension()
{
	return &luaExtension;
}
#endif

template <typename FunctionType, FunctionType function>
Function<FunctionType, function>::Function(const char* name) : FunctionBase(name) {}

END_INANITY_META

#endif
