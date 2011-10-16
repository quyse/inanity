#ifndef ___INANITY_SCRIPTING_HPP___
#define ___INANITY_SCRIPTING_HPP___

/* Файл задаёт, какой скриптовой системой пользоваться.
 * В данный момент это Lua.
 * В будущем можно добавить ещё другую, а здесь сделать
 * возможность выбора на этапе компиляции.
 */

#include "Object.hpp"

#ifdef ___INANITY_SCRIPTING

#include "LuaState.hpp"

BEGIN_INANITY

/// Тип, хранящий состояние скриптовой системы.
typedef LuaState ScriptState;
/// Тип, используемый для вызовов.
typedef LuaCall ScriptCall;
/// Тип метода, которому должны соответствовать скриптовые методы классов.
typedef LuaMethod ScriptMethod;
/// Тип, используемый для указания метаинформации о типах, видимых скриптам.
typedef LuaClass ScriptClass;
/// Тип скриптов.
typedef LuaScript Script;

//****** Полезные макросы для определения методов и свойств.

// Добавить в определение класса, который может быть представлен в скрипте.
#define SCRIPTABLE_CLASS() \
	static ScriptClass scriptClass; \
	static ScriptClass InitScriptClass()
#define SCRIPTABLE_CALL(name) \
	void Script_##name(ScriptCall& call)
// Карта методов - добавить в cpp-файл класса.
#define SCRIPTABLE_MAP_BEGIN(cls) \
	ScriptClass cls::scriptClass = cls::InitScriptClass(); \
	ScriptClass cls::InitScriptClass() { ScriptClass res
#define SCRIPTABLE_MAP_END() \
	return res; }
// Метод в карте методов.
#define SCRIPTABLE_METHOD(cls, method) \
	res.AddMethod(#method, static_cast<ScriptMethod>(&cls::Script_##method))

END_INANITY

#endif

#endif
