#ifndef ___INANITY_META_CONSTRUCTOR_HPP___
#define ___INANITY_META_CONSTRUCTOR_HPP___

#include "Extension.hpp"
#include "../script/lua/extension.hpp"
#include "../script/v8/extension.hpp"

BEGIN_INANITY_META

class ConstructorBase
{
public:
	virtual Script::Lua::ConstructorExtensionBase* GetLuaExtension() = 0;
	virtual Script::V8::ConstructorExtensionBase* GetV8Extension() = 0;
};

template <typename ConstructorType>
class Constructor;

END_INANITY_META

#endif
