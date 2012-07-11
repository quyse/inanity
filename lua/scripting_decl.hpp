#ifndef ___INANITY_LUA_SCRIPTING_DECL_HPP___
#define ___INANITY_LUA_SCRIPTING_DECL_HPP___

#include "reflection_decl.hpp"

// Добавить в определение класса, который может быть представлен в скрипте.
#define SCRIPTABLE_CLASS(className) \
	static Inanity::Lua::ConcreteClass<className> scriptClass

#endif
