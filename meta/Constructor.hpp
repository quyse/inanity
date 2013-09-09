#ifndef ___INANITY_META_CONSTRUCTOR_HPP___
#define ___INANITY_META_CONSTRUCTOR_HPP___

#include "Extension.hpp"
#include "../script/lua/extension.hpp"

BEGIN_INANITY_META

class ConstructorBase
{
public:
	virtual Script::Lua::ConstructorExtensionBase* GetLuaExtension() = 0;
};

template <typename ConstructorType>
class Constructor;

END_INANITY_META

#endif
