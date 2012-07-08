#ifndef ___INANITY_LUA_STUFF_HPP___
#define ___INANITY_LUA_STUFF_HPP___

#include "lua.hpp"
#include "lualib.hpp"
#include "reflection_decl.hpp"

BEGIN_INANITY_LUA

/// Универсальная функция для индексирования метатаблиц (классов и объектов).
/** Предполагает, что в замыкании лежит таблица методов. */
int MetaTable_index(lua_State* state);

/// Запихать в стек метатаблицу для классов.
/** Метатаблица классов нигде не хранится, и создаётся каждый раз заново.
Потому что предполагается, что она один раз будет нужна (для одного глобального объекта).
Вообще говоря, можно было бы обойтись вообще без метатаблицы, а сделать просто таблицу,
а не объект. Но пофиг, пусть будет более круто. */
void PushClassMetaTable(lua_State* state, Class& cls);

/// Зарегистрировать класс в состоянии.
void RegisterClass(lua_State* state, Class& cls);

/// Структура с методами метатаблицы объектов.
int ObjectMetaTable_gc(lua_State* state);

/// Запихать в стек метатаблицу для объектов.
void PushObjectMetaTable(lua_State* state, Class& cls);

/// Обработать ошибку в Lua.
void ProcessError(lua_State* state);

/// Описать строкой значение в стеке Lua.
String DescribeValue(lua_State* state, int index);

END_INANITY_LUA

#endif
