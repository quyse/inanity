#ifndef ___INANITY_META_METHOD_HPP___
#define ___INANITY_META_METHOD_HPP___

#include "Extension.hpp"

#ifdef ___INANITY_META_LUA___
#include "../script/lua/extension.hpp"
#endif

BEGIN_INANITY_META

class MethodBase
{
protected:
	const char* name;

	MethodBase(const char* name);

public:
	const char* GetName() const;

#ifdef ___INANITY_META_LUA___
	virtual Script::Lua::MethodExtensionBase* GetLuaExtension() = 0;
#endif
};

template <typename MethodType, MethodType method>
class Method : public MethodBase
{
#ifdef ___INANITY_META_LUA___
private:
	Script::Lua::Extension<Method<MethodType, method> > luaExtension;
public:
	Script::Lua::MethodExtensionBase* GetLuaExtension();
#endif

public:
	Method(const char* name);
};

END_INANITY_META

#endif
