#ifndef ___INANITY_LUA_FUNCTION_HPP___
#define ___INANITY_LUA_FUNCTION_HPP___

#include "lua.hpp"
#include "Calling.hpp"

BEGIN_INANITY_LUA

class State;

/// Класс функции Lua.
/** Хранит собственно функцию в Lua Registry, по адресу объекта Function. */
class Function : public Object
{
private:
	ptr<State> state;

public:
	/// Создать скрипт Lua.
	/** Предполагается, что при вызове в стеке Lua лежит функция скрипта. Конструктор забирает её из стека. */
	Function(ptr<State> state);
	~Function();

	/// Вызвать функцию с заданными параметрами.
	template <typename ReturnType, typename... ArgTypes>
	ReturnType Run(ArgTypes... args)
	{
		PushArguments<ArgTypes...>(args...);
		lua_call(state->state, 
	}
};

END_INANITY_LUA

#endif
