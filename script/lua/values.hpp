#ifndef ___INANITY_SCRIPT_LUA_VALUES_HPP___
#define ___INANITY_SCRIPT_LUA_VALUES_HPP___

#include "lua.hpp"

BEGIN_INANITY_LUA

/* Все структуры значений (value) содержат следующие методы:
Получить значение из стека.
static T Get(lua_State* luaState, int index);
Положить значение в стек.
static void Push(lua_State* luaState, T value);
*/

template <typename T>
struct Value;

END_INANITY_LUA

#endif
