#ifndef ___INANITY_SCRIPTING_HPP___
#define ___INANITY_SCRIPTING_HPP___

/* Файл для включения в cpp-файлы.
*/

#include "Object.hpp"

#define ___INANITY_SCRIPTING
#ifdef ___INANITY_SCRIPTING

#include "lua/reflection.hpp"
#include "lua/State.hpp"
#include <typeinfo>

BEGIN_INANITY

/// Тип, хранящий состояние скриптовой системы.
typedef Lua::State ScriptState;
/// Тип скриптов.
typedef Lua::Script Script;

//****** Макросы для определения методов и свойств.

// Добавить в файл реализации.
#define SCRIPTABLE_MAP_BEGIN(className) \
	ScriptClass className::scriptClass = className::InitScriptClass(); \
	ScriptClass className::InitScriptClass() { ScriptClass res(typeid(className).name())
#define SCRIPTABLE_MAP_END() \
	return res; }
// Метод в карте методов.
#define SCRIPTABLE_METHOD(className, methodName) \
	res.AddMethod<decltype(&className::methodName), &className::methodName>(#methodName)

END_INANITY

#else // ___INANITY_SCRIPTING

#define SCRIPTABLE_MAP_BEGIN(className)
#define SCRIPTABLE_MAP_END()
#define SCRIPTABLE_METHOD(className, methodName)

#endif // ___INANITY_SCRIPTING

#endif
