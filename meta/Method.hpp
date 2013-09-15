#ifndef ___INANITY_META_METHOD_HPP___
#define ___INANITY_META_METHOD_HPP___

#include "Extension.hpp"

#include "../script/lua/extension.hpp"
#include "../script/v8/extension.hpp"

BEGIN_INANITY_META

class MethodBase
{
protected:
	const char* name;

	MethodBase(const char* name);

public:
	const char* GetName() const;

	virtual Script::Lua::MethodExtensionBase* GetLuaExtension() = 0;
	virtual Script::V8::MethodExtensionBase* GetV8Extension() = 0;
};

template <typename MethodType, MethodType method>
class Method;

END_INANITY_META

#endif
