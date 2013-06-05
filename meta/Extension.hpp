#ifndef ___INANITY_META_EXTENSION_HPP___
#define ___INANITY_META_EXTENSION_HPP___

#include "meta.hpp"

#ifdef ___INANITY_META_LUA___
#include "../script/extension.hpp"
#endif

BEGIN_INANITY_META

struct ExtensionBase
{
#ifdef ___INANITY_META_LUA___
	virtual GetExtension()
};

/// Struct contains enabled compile-time extension substructs.
template <typename MetaType>
struct Extension : public ExtensionBase
{
#ifdef ___INANITY_META_LUA___
	Script::Lua::Extension<MetaType> lua;
#endif
};

END_INANITY_META

#endif
