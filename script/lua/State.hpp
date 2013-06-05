#ifndef ___INANITY_SCRIPT_LUA_STATE_HPP___
#define ___INANITY_SCRIPT_LUA_STATE_HPP___

/* Файл содержит общие определения, используемые для поддержки Lua.
 */

#include "lua.hpp"
#include "lualib.hpp"
#include "stuff.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_LUA

class Script;

/// Класс, инкапсулирующий состояние Lua.
class State : public Object
{
private:
	lua_State* state;

private:
	/// Обработчик выделения памяти Lua.
	static void* Alloc(void* self, void* ptr, size_t osize, size_t nsize);
	/// Обработчик фатальной ошибки Lua.
	static int Panic(lua_State* state);

public:
	State();
	~State();

	/// Получить состояние Lua.
	lua_State* GetState();

	/// Зарегистрировать класс.
	template <typename ClassType>
	void RegisterClass()
	{
		Lua::RegisterCls(state, ClassType::scriptClass);
	}

	/// Загрузить скрипт из файла.
	ptr<Script> LoadScript(ptr<File> file);
};

END_INANITY_LUA

#endif
