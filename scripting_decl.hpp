#ifndef ___INANITY_SCRIPTING_DECL_HPP___
#define ___INANITY_SCRIPTING_DECL_HPP___

/* Файл задаёт, какой скриптовой системой пользоваться.
В данный момент это Lua.
В будущем можно добавить ещё другую, а здесь сделать
возможность выбора на этапе компиляции.

Данный файл нужно включать в заголовочные файлы.
В cpp-файл нужно включать scripting.hpp.
*/

#include "Object.hpp"

#define ___INANITY_SCRIPTING
#ifdef ___INANITY_SCRIPTING

#include "lua/reflection_decl.hpp"

BEGIN_INANITY

/// Тип, используемый для указания метаинформации о типах, видимых скриптам.
typedef Lua::Class ScriptClass;

// Добавить в определение класса, который может быть представлен в скрипте.
#define SCRIPTABLE_CLASS() \
	static ScriptClass scriptClass; \
	static ScriptClass InitScriptClass()

END_INANITY

#else // ___INANITY_SCRIPTING

#define SCRIPTABLE_CLASS()

#endif // ___INANITY_SCRIPTING

#endif
