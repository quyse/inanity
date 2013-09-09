#ifndef ___INANITY_META_CONSTRUCTOR_IPP___
#define ___INANITY_META_CONSTRUCTOR_IPP___

#include "Constructor.hpp"
#include "Callable.hpp"

BEGIN_INANITY_META

template <typename ConstructorType>
class Constructor : public ConstructorBase
{
private:
#ifdef ___INANITY_META_LUA___
	Script::Lua::Extension<Constructor<ConstructorType> > luaExtension;
#endif

public:
	Script::Lua::ConstructorExtensionBase* GetLuaExtension()
	{
#ifdef ___INANITY_META_LUA___
		return &luaExtension;
#else
		return 0;
#endif
	}
};

END_INANITY_META

#endif
