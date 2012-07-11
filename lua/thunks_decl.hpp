#ifndef ___INANITY_LUA_THUNKS_DECL_HPP___
#define ___INANITY_LUA_THUNKS_DECL_HPP___

#include "lua.hpp"
#include "lualib.hpp"

BEGIN_INANITY_LUA

/// Структура, сохраняющая состояние для получения аргументов.
struct ArgGettingState
{
	/// Состояние Lua.
	lua_State* state;
	/// Общее количество аргументов.
	int argsCount;
	/// Количество уже вытащенных аргументов.
	int gotArgsCount;
};

/// Специальный бесполезный тип, который нужен для указания конструкторов.
struct Construct;

/// Переходник для функций.
/** Предназначен для содержания следующего метода:
static int Thunk(lua_State* luaState);
Этот метод должен выполнять роль переходника между Lua и указанной функцией/методом,
забирая аргументы из стека Lua, и возвращая в стеке результат вызова.
*/
template <typename CalleeType, CalleeType callee>
struct CalleeThunk;

/// Переходник для конструкторов.
/* CalleeType = Construct (ClassType::*)(ArgTypes...)
*/
template <typename CalleeType>
struct ConstructorThunk;

END_INANITY_LUA

#endif
