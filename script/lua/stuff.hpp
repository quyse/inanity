#ifndef ___INANITY_SCRIPT_LUA_STUFF_HPP___
#define ___INANITY_SCRIPT_LUA_STUFF_HPP___

#include "lua.hpp"
#include "MetaProvider.hpp"
#include "lualib.hpp"
#include "../../String.hpp"
#include <iostream>

BEGIN_INANITY

class Exception;

END_INANITY

BEGIN_INANITY_LUA

/// General routine which indexes metatables (of classes and objects).
/** Assumes that closure contains methods table. */
int MetaTable_index(lua_State* state);

/// Push class metatable in stack.
void PushClassMetaTable(lua_State* state, MetaProvider::ClassBase* cls);

/// Register a class in Lua state.
void RegisterClassMeta(lua_State* state, MetaProvider::ClassBase* cls);

/// Структура с методами метатаблицы объектов.
int ObjectMetaTable_gc(lua_State* state);

/// Reclaim object from userdata on top of the stack.
void ReclaimObjectFromUserData(lua_State* state);
/// Reclaim object by pointer.
void ReclaimObject(lua_State* state, RefCounted* object);

/// Push object metatable in stack.
void PushObjectMetaTable(lua_State* state, MetaProvider::ClassBase* cls);

/// Converts Lua error to exception.
/** Pops an error out from stack. */
ptr<Exception> ErrorToException(lua_State* state);
/// Process an error in Lua.
/** Pops an error from stack and throws it as an exception. */
void ProcessError(lua_State* state);
/// Processes a script call error, and return extended information about it in Lua.
int ScriptErrorHook(lua_State* state);

/// Convert Lua value to string.
void DescribeValue(lua_State* state, int index, std::ostream& stream);

END_INANITY_LUA

#endif
