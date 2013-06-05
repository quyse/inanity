#ifndef ___INANITY_SCRIPT_LUA_SCRIPTING_DECL_HPP___
#define ___INANITY_SCRIPT_LUA_SCRIPTING_DECL_HPP___

#include "reflection.hpp"

// Добавить в определение класса, который может быть представлен в скрипте.
#define SCRIPTABLE_CLASS(className) \
	static Inanity::Lua::ConcreteClass<className> scriptClass

#endif
