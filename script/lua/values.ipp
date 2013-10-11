#ifndef ___INANITY_SCRIPT_LUA_VALUES_IPP___
#define ___INANITY_SCRIPT_LUA_VALUES_IPP___

#include "values.hpp"
#include "Any.hpp"
#include "State.hpp"
#include "stuff.hpp"
#include "userdata.hpp"
#include "lualib.hpp"
#include "../../Exception.hpp"
#include <sstream>

BEGIN_INANITY_LUA

template <>
struct Value<bool>
{
	typedef bool ValueType;

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
	typedef int ValueType;

	static inline int Get(lua_State* state, int index)
	{
		int isnum;
		lua_Integer res = lua_tointegerx(state, index, &isnum);
		if(!isnum)
			THROW("Expected an integer for argument");
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
	typedef double ValueType;

	static inline double Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW("Expected a double for argument");
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
	typedef float ValueType;

	static inline float Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW("Expected a float for argument");
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
	typedef unsigned int ValueType;

	static inline unsigned int Get(lua_State* state, int index)
	{
		int isnum;
		lua_Integer res = lua_tointegerx(state, index, &isnum);
		if(!isnum)
			THROW("Expected an integer for argument");
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
	typedef long long ValueType;

	static inline long long Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW("Expected an integer for argument");
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
	typedef unsigned long long ValueType;

	static inline unsigned long long Get(lua_State* state, int index)
	{
		int isnum;
		lua_Number res = lua_tonumberx(state, index, &isnum);
		if(!isnum)
			THROW("Expected an integer for argument");
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
	typedef const char* ValueType;

	static inline const char* Get(lua_State* state, int index)
	{
		const char* res = lua_tostring(state, index);
		if(!res)
			THROW("Expected a string for argument");
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
	typedef String ValueType;

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
	typedef String ValueType;

	static inline String Get(lua_State* state, int index)
	{
		return Value<const char*>::Get(state, index);
	}
};

template <>
struct Value<ptr<Script::Any> >
{
	typedef ptr<Script::Any> ValueType;

	static inline ptr<Script::Any> Get(lua_State* state, int index)
	{
		lua_pushvalue(state, index);
		return State::GetStateByLuaState(state)->CreateAny();
	}

	static void Push(lua_State* state, ptr<Script::Any> value)
	{
		fast_cast<Any*>(&*value)->PushValue();
	}
};

template <typename ObjectType>
struct Value<ptr<ObjectType> >
{
	typedef ptr<ObjectType> ValueType;

	static inline ptr<ObjectType> Get(lua_State* state, int index)
	{
		// проверить, если это nil (это допустимо)
		if(lua_isnil(state, index))
			return 0;

		// получить userdata для объекта, и проверить, что это объект
		ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, index);
		if(!userData || lua_islightuserdata(state, index) || userData->type != UserData::typeObject)
		{
			const char* fullClassName = Meta::MetaOf<MetaProvider, ObjectType>()->GetFullName();
			std::ostringstream stream;
			stream
				<< "Expected an object of type '"
				<< fullClassName
				<< "' for argument, but got ";
			DescribeValue(state, index, stream);
			THROW(stream.str());
		}

		// проверить тип объекта, в случае необходимости привести к вышестоящему типу
		for(MetaProvider::ClassBase* cls = userData->cls; cls; cls = cls->GetParent())
			if(cls == Meta::MetaOf<MetaProvider, ObjectType>())
				// вернуть объект
				return (ObjectType*)userData->object;
		// если здесь, значит, мы проверили всю цепочку наследования, а тип не нашли
		const char* fullClassName = Meta::MetaOf<MetaProvider, ObjectType>()->GetFullName();
		THROW(String("Can't cast object of type '") + userData->cls->GetFullName() + "' to expected type '" + fullClassName + "'");
	}

	static inline void Push(lua_State* state, ptr<ObjectType> value)
	{
		// проверить, если указатель нулевой, то запустить nil (это допустимо)
		if(!value)
		{
			lua_pushnil(state);
			return;
		}

		// попробовать найти существующее userdata для объекта
		lua_pushlightuserdata(state, (ObjectType*)value);		// value
		lua_gettable(state, LUA_REGISTRYINDEX);							// userdata
		// если нет, сделать
		if(lua_isnil(state, -1))
		{
			// выбросить nil из стека
			lua_pop(state, 1);																//

			ObjectUserData* userData = (ObjectUserData*)lua_newuserdata(state, sizeof(ObjectUserData));
			userData->type = UserData::typeObject;
			userData->object = (RefCounted*)(ObjectType*)value;
			userData->cls = Meta::MetaOf<MetaProvider, ObjectType>();
			// указать метатаблицу
			PushObjectMetaTable(state, userData->cls);
			lua_setmetatable(state, -2);
			// задать дополнительную ссылку объекту
			userData->object->Reference();

			// положить адрес объекта
			lua_pushlightuserdata(state, (ObjectType*)value);
			// продублировать userdata в стеке
			lua_pushvalue(state, -2);
			// сохранить userdata по адресу объекта, для повторного использования
			lua_settable(state, LUA_REGISTRYINDEX);
		}
	}
};

template <typename T>
struct Value
{
	typedef T ValueType;

	static inline T Get(lua_State* state, int index)
	{
		lua_pushvalue(state, index);
		return ConvertFromScript<T>(State::GetStateByLuaState(state)->CreateAny());
	}

	static inline void Push(lua_State* state, const T& value)
	{
		fast_cast<Any*>(&*ConvertToScript<T>(State::GetStateByLuaState(state), value))->PushValue();
	}
};

template <typename T>
struct Value<const T&>
{
	typedef T ValueType;

	static inline T Get(lua_State* state, int index)
	{
		lua_pushvalue(state, index);
		return ConvertFromScript<T>(State::GetStateByLuaState(state)->CreateAny());
	}

	static inline void Push(lua_State* state, const T& value)
	{
		fast_cast<Any*>(&*ConvertToScript<T>(State::GetStateByLuaState(state), value))->PushValue();
	}
};

END_INANITY_LUA

#endif
