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
/// Тип, используемый для указания метаинформации о типах, видимых скриптам.
typedef LuaClass ScriptClass;
/// Тип скриптов.
typedef LuaScript Script;

//****** Макросы для определения методов и свойств.

// Добавить в определение класса, который может быть представлен в скрипте.
#define SCRIPTABLE_CLASS() \
	static ScriptClass scriptClass; \
	static ScriptClass InitScriptClass()
// Добавить в файл реализации.
#define SCRIPTABLE_MAP_BEGIN(className) \
	ScriptClass className::scriptClass = cls::InitScriptClass(); \
	ScriptClass className::InitScriptClass() { ScriptClass res
#define SCRIPTABLE_MAP_END() \
	return res; }
// Метод в карте методов.
#define SCRIPTABLE_METHOD(className, methodName) \
	res.AddMethod(#method, &className::methodName)

END_INANITY

#endif

#endif
