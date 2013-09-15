#ifndef ___INANITY_META_FUNCTION_HPP___
#define ___INANITY_META_FUNCTION_HPP___

#include "Extension.hpp"

#include "../script/lua/extension.hpp"
#include "../script/v8/extension.hpp"

BEGIN_INANITY_META

class FunctionBase
{
protected:
	const char* name;

	FunctionBase(const char* name);

public:
	const char* GetName() const;

	virtual Script::Lua::FunctionExtensionBase* GetLuaExtension() = 0;
	virtual Script::V8::FunctionExtensionBase* GetV8Extension() = 0;
};

template <typename FunctionType, FunctionType function>
class Function;

END_INANITY_META

#endif
