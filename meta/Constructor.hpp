#ifndef ___INANITY_META_CONSTRUCTOR_HPP___
#define ___INANITY_META_CONSTRUCTOR_HPP___

#include "Extension.hpp"

#ifdef ___INANITY_META_LUA___
#include "../script/lua/extension.hpp"
#endif

BEGIN_INANITY_META

class ConstructorBase
{
public:
#ifdef ___INANITY_META_LUA___
	virtual Script::Lua::ConstructorExtensionBase* GetLuaExtension() = 0;
#endif
};

template <typename ConstructorType>
class Constructor : public ConstructorBase
{
#ifdef ___INANITY_META_LUA___
private:
	Script::Lua::Extension<Constructor<ConstructorType> > luaExtension;
public:
	Script::Lua::ConstructorExtensionBase* GetLuaExtension();
#endif
};

END_INANITY_META

#endif
