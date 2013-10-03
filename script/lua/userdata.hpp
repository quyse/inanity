#ifndef ___INANITY_SCRIPT_LUA_USERDATA_HPP___
#define ___INANITY_SCRIPT_LUA_USERDATA_HPP___

#include "lua.hpp"

/* Файл содержит определения структур пользовательских данных
(userdata), которые хранят в Lua различные вещи.
*/

BEGIN_INANITY_META

END_INANITY_META

BEGIN_INANITY_LUA

/// Каждая открытая userdata является этой структурой.
struct UserData
{
	/// Тип userdata.
	enum Type
	{
		typeClass,
		typeObject,
	} type;
};

/// Структура объекта, представляющего класс в Lua.
struct ClassUserData : public UserData
{
};

/// Структура объекта, представляющего управляемый указатель в Lua.
struct ObjectUserData : public UserData
{
	RefCounted* object;
	Meta::ClassBase* cls;
};

END_INANITY_LUA

#endif
