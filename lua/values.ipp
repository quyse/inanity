#ifndef ___INANITY_LUA_VALUES_IPP___
#define ___INANITY_LUA_VALUES_IPP___

#include "values.hpp"
#include "reflection.hpp"
#include "stuff.hpp"
#include "userdata.hpp"
#include "lualib.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_LUA

template <>
struct Value<bool>
{
	static inline bool Get(lua_State* state, int index)
	{
		return !!lua_toboolean(state, index);
	}

	static inline void Push(lua_State* state, bool value)
	{
		lua_pushboolean(state, (int)value);
	}
};

template <>
struct Value<int>
{
	static inline int Get(lua_State* state, int index)
	{
		int isnum;
		lua_Integer res = lua_tointegerx(state, index, &isnum);
		if(!isnum)
			THROW_PRIMARY_EXCEPTION("Expected an integer for argument");
		return (int)res;
	}

	static inline void Push(lua_State* state, int value)
	{
		lua_pushinteger(state, (lua_Integer)value);
	}
};

template <>
struct Value<double>
{
	static inline double Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW_PRIMARY_EXCEPTION("Expected a double for argument");
		return (double)res;
	}

	static inline void Push(lua_State* state, double value)
	{
		lua_pushnumber(state, (lua_Number)value);
	}
};

template <>
struct Value<float>
{
	static inline float Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW_PRIMARY_EXCEPTION("Expected a float for argument");
		return (float)res;
	}

	static inline void Push(lua_State* state, float value)
	{
		lua_pushnumber(state, (lua_Number)value);
	}
};

template <>
struct Value<unsigned int>
{
	static inline unsigned int Get(lua_State* state, int index)
	{
		int isnum;
		lua_Integer res = lua_tointegerx(state, index, &isnum);
		if(!isnum)
			THROW_PRIMARY_EXCEPTION("Expected an integer for argument");
		return (unsigned int)res;
	}

	static inline void Push(lua_State* state, unsigned int value)
	{
		lua_pushinteger(state, (lua_Integer)value);
	}
};

template <>
struct Value<long long>
{
	static inline long long Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW_PRIMARY_EXCEPTION("Expected an integer for argument");
		return (long long)res;
	}

	static inline void Push(lua_State* state, long long value)
	{
		lua_pushnumber(state, (lua_Number)value);
	}
};

template <>
struct Value<unsigned long long>
{
	static inline unsigned long long Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW_PRIMARY_EXCEPTION("Expected an integer for argument");
		return (unsigned long long)res;
	}

	static inline void Push(lua_State* state, unsigned long long value)
	{
		lua_pushnumber(state, (lua_Number)value);
	}
};

template <>
struct Value<const char*>
{
	static inline const char* Get(lua_State* state, int index)
	{
		const char* res = lua_tostring(state, index);
		if(!res)
			THROW_PRIMARY_EXCEPTION("Expected a string for argument");
		return res;
	}

	static inline void Push(lua_State* state, const char* value)
	{
		lua_pushstring(state, value);
	}
};

template <>
struct Value<String>
{
	static inline String Get(lua_State* state, int index)
	{
		return Value<const char*>::Get(state, index);
	}

	static inline void Push(lua_State* state, String value)
	{
		lua_pushstring(state, value.c_str());
	}
};

template <>
struct Value<const String&>
{
	static inline String Get(lua_State* state, int index)
	{
		return Value<const char*>::Get(state, index);
	}
};

template <typename ObjectType>
struct Value<ptr<ObjectType> >
{
	static inline ptr<ObjectType> Get(lua_State* state, int index)
	{
		// проверить, если это nil (это допустимо)
		if(lua_isnil(state, index))
			return 0;

		// получить userdata для объекта, и проверить, что это объект
		ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, index);
		if(!userData || lua_islightuserdata(state, index) || userData->type != UserData::typeObject)
			THROW_PRIMARY_EXCEPTION("Expected an object of type '" + ObjectType::scriptClass.GetFullName() + "' for argument, but got " + DescribeValue(state, index));

		// проверить тип объекта, в случае необходимости привести к вышестоящему типу
		for(Class* cls = userData->cls; cls; cls = cls->GetParent())
			if(cls == &ObjectType::scriptClass)
				// вернуть объект
				return (ObjectType*)userData->object;
		// если здесь, значит, мы проверили всю цепочку наследования, а тип не нашли
		THROW_PRIMARY_EXCEPTION("Can't cast object of type '" + userData->cls->GetFullName() + "' to expected type '" + ObjectType::scriptClass.GetFullName() + "'");
	}

	static inline void Push(lua_State* state, ptr<ObjectType> value)
	{
		// проверить, если указатель нулевой, то запустить nil (это допустимо)
		if(!value)
		{
			lua_pushnil(state);
			return;
		}

		ObjectUserData* userData = (ObjectUserData*)lua_newuserdata(state, sizeof(ObjectUserData));
		userData->type = UserData::typeObject;
		userData->object = (Object*)(ObjectType*)value;
		userData->cls = &ObjectType::scriptClass;
		// указать метатаблицу
		PushObjectMetaTable(state, ObjectType::scriptClass);
		lua_setmetatable(state, -2);
		// задать дополнительную ссылку объекту
		userData->object->Reference();
	}
};

END_INANITY_LUA

#endif
