#ifndef ___INANITY_SCRIPTING_HPP___
#define ___INANITY_SCRIPTING_HPP___

/* Файл задаёт, какой скриптовой системой пользоваться.
 * В данный момент это Lua.
 * В будущем можно добавить ещё другую, а здесь сделать
 * возможность выбора на этапе компиляции.
 */

#include "LuaState.hpp"

typedef LuaState ScriptState;
typedef LuaCall ScriptCall;
typedef LuaClass ScriptClass;

#endif
