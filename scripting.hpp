#ifndef ___INANITY_SCRIPTING_HPP___
#define ___INANITY_SCRIPTING_HPP___

/* Файл задаёт, какой скриптовой системой пользоваться.
 * В данный момент это Lua.
 * В будущем можно добавить ещё другую, а здесь сделать
 * возможность выбора на этапе компиляции.
 */

#include "Object.hpp"

#define ___INANITY_SCRIPTING
#ifdef ___INANITY_SCRIPTING

#include "inanity-lua.hpp"

BEGIN_INANITY

/// Тип, хранящий состояние скриптовой системы.
typedef Lua::State ScriptState;
/// Тип, используемый для указания метаинформации о типах, видимых скриптам.
typedef Lua::Class ScriptClass;
/// Тип скриптов.
typedef Lua::Script Script;

//****** Макросы для определения методов и свойств.

// Добавить в определение класса, который может быть представлен в скрипте.
#define SCRIPTABLE_CLASS() \
	static ScriptClass scriptClass; \
	static ScriptClass InitScriptClass()
// Добавить в файл реализации.
#define SCRIPTABLE_MAP_BEGIN(className) \
	ScriptClass className::scriptClass = cls::InitScriptClass(); \
	ScriptClass className::InitScriptClass() { ScriptClass res(typeid(className)->name)
#define SCRIPTABLE_MAP_END() \
	return res; }
// Метод в карте методов.
#define SCRIPTABLE_METHOD(className, methodName) \
	res.AddMethod(#methodName, &className::methodName)

END_INANITY

#else // ___INANITY_SCRIPTING

#define SCRIPTABLE_CLASS()
#define SCRIPTABLE_MAP_BEGIN(className)
#define SCRIPTABLE_MAP_END()
#define SCRIPTABLE_METHOD(className, methodName)

#endif // ___INANITY_SCRIPTING

#endif
