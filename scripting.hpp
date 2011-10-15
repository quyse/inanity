#ifndef ___INANITY_SCRIPTING_HPP___
#define ___INANITY_SCRIPTING_HPP___

/* Файл задаёт, какой скриптовой системой пользоваться.
 * В данный момент это Lua.
 * В будущем можно добавить ещё другую, а здесь сделать
 * возможность выбора на этапе компиляции.
 */

#include "LuaState.hpp"

BEGIN_INANITY

/// Тип, хранящий состояние скриптовой системы.
typedef LuaState ScriptState;
/// Тип, используемый для вызовов.
typedef LuaCall ScriptCall;
/// Тип, используемый для указания метаинформации о типах, видимых скриптам.
typedef LuaClass ScriptClass;
/// Тип скриптов.
typedef LuaScript Script;

END_INANITY

#endif
