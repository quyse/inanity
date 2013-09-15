#ifndef ___INANITY_META_FUNCTION_IPP___
#define ___INANITY_META_FUNCTION_IPP___

#include "Function.hpp"

BEGIN_INANITY_META

template <typename FunctionType, FunctionType function>
class Function : public FunctionBase
{
private:

#ifdef ___INANITY_META_LUA___
	Script::Lua::Extension<Function<FunctionType, function> > luaExtension;
#endif

#ifdef ___INANITY_META_V8___
	Script::V8::Extension<Function<FunctionType, function> > v8Extension;
#endif

public:

	Script::Lua::FunctionExtensionBase* GetLuaExtension()
	{
#ifdef ___INANITY_META_LUA___
		return &luaExtension;
#else
		return 0;
#endif
	}

	Script::V8::FunctionExtensionBase* GetV8Extension()
	{
#ifdef ___INANITY_META_V8___
		return &v8Extension;
#else
		return 0;
#endif
	}

public:
	Function(const char* name) : FunctionBase(name) {}
};

END_INANITY_META

#endif
