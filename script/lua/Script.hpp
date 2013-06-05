#ifndef ___INANITY_SCRIPT_LUA_SCRIPT_HPP___
#define ___INANITY_SCRIPT_LUA_SCRIPT_HPP___

#include "lua.hpp"

BEGIN_INANITY_LUA

class State;

/// Класс функции Lua.
/** Хранит собственно функцию в Lua Registry, по адресу объекта Script. */
class Script : public Object
{
private:
	ptr<State> state;

public:
	/// Создать скрипт Lua.
	/** Предполагается, что при вызове в стеке Lua лежит функция скрипта. Конструктор забирает её из стека. */
	Script(ptr<State> state);
	~Script();

	/// Вызвать скрипт.
	template <typename ReturnType>
	ReturnType Run();
};

END_INANITY_LUA

#endif
