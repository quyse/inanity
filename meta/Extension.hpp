#ifndef ___INANITY_META_EXTENSION_HPP___
#define ___INANITY_META_EXTENSION_HPP___

#include "meta.hpp"

#ifdef ___INANITY_META_LUA___
#include "../script/lua/extension.hpp"
#endif

BEGIN_INANITY_META

/// Struct contains enabled compile-time extension substructs.
template <typename MetaType>
struct Extension
{
#ifdef ___INANITY_META_LUA___
	Script::Lua::Extension<MetaType> lua;
#endif
};

END_INANITY_META

#endif
