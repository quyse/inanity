#ifndef ___INANITY_META_FUNCTION_HPP___
#define ___INANITY_META_FUNCTION_HPP___

#include "Extension.hpp"

#ifdef ___INANITY_META_LUA___
#include "../script/lua/extension.hpp"
#endif

BEGIN_INANITY_META

class FunctionBase
{
protected:
	const char* name;

	FunctionBase(const char* name);

public:
	const char* GetName() const;

#ifdef ___INANITY_META_LUA___
	virtual Script::Lua::FunctionExtensionBase* GetLuaExtension() = 0;
#endif
};

template <typename FunctionType, FunctionType function>
class Function : public FunctionBase
{
#ifdef ___INANITY_META_LUA___
private:
	Script::Lua::Extension<Function<FunctionType, function> > luaExtension;
public:
	Script::Lua::FunctionExtensionBase* GetLuaExtension();
#endif

public:
	Function(const char* name);
};

END_INANITY_META

#endif
