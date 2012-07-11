#ifndef ___INANITY_SCRIPTING_DECL_HPP___
#define ___INANITY_SCRIPTING_DECL_HPP___

/* Файл задаёт, какой скриптовой системой пользоваться.
В данный момент это Lua.
В будущем можно добавить ещё другую, а здесь сделать
возможность выбора на этапе компиляции.

Файл, который нужно использовать в заголовочных файлах классов,
которые могут использоваться в скриптах.
*/

#include "Object.hpp"

#define ___INANITY_SCRIPTING
#ifdef ___INANITY_SCRIPTING

#include "lua/scripting_decl.hpp"

#else

#define SCRIPTABLE_CLASS(className)

#endif

#endif
