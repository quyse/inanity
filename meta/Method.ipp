#ifndef ___INANITY_META_METHOD_IPP___
#define ___INANITY_META_METHOD_IPP___

#include "Method.hpp"
#include "Callable.ipp"

BEGIN_INANITY_META

template <typename MethodType, MethodType method>
class Method : public MethodBase
{
private:
#ifdef ___INANITY_META_LUA___
	Script::Lua::Extension<Method<MethodType, method> > luaExtension;
#endif

public:
	Script::Lua::MethodExtensionBase* GetLuaExtension()
	{
#ifdef ___INANITY_META_LUA___
		return &luaExtension;
#else
		return 0;
#endif
	}

public:
	Method(const char* name) : MethodBase(name)
	{
		// register the method in the class
		typedef typename Callable<MethodType>::ClassType ClassType;
		ClassType::GetMeta()->AddMethod(this);
	}
};

END_INANITY_META

#endif
