#ifndef ___INANITY_META_CLASS_IPP___
#define ___INANITY_META_CLASS_IPP___

#include "Class.hpp"

BEGIN_INANITY_META

/// Stores metainformation about C++ class.
template <typename ClassType>
class Class : public ClassBase
{
private:

#ifdef ___INANITY_META_LUA___
	Script::Lua::Extension<Class<ClassType> > luaExtension;
#endif

#ifdef ___INANITY_META_V8___
	Script::V8::Extension<Class<ClassType> > v8Extension;
#endif

public:

	Script::Lua::ClassExtensionBase* GetLuaExtension()
	{
#ifdef ___INANITY_META_LUA___
		return &luaExtension;
#else
		return 0;
#endif
	}

	Script::V8::ClassExtensionBase* GetV8Extension()
	{
#ifdef ___INANITY_META_V8___
		return &v8Extension;
#else
		return 0;
#endif
	}

public:
	Class(const char* name, const char* fullName)
	: ClassBase(name, fullName) {}
};

int InanityMetaClassIppShouldBeOne = 0;

END_INANITY_META

#endif
