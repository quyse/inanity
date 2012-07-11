#ifndef ___INANITY_SCRIPTING_HPP___
#define ___INANITY_SCRIPTING_HPP___

/* Файл для включения, если требуется непосредственная работа со скриптами.
*/

#include "scripting_decl.hpp"

#ifdef ___INANITY_SCRIPTING

#include "lua/reflection.hpp"
#include "lua/callable.hpp"
#include "lua/thunks.hpp"
#include "lua/State.hpp"
#include "lua/Script.hpp"

BEGIN_INANITY

typedef Lua::State ScriptState;
typedef Lua::Script Script;

END_INANITY

#endif // ___INANITY_SCRIPTING

#endif
