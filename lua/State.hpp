#ifndef ___INANITY_LUA_STATE_HPP___
#define ___INANITY_LUA_STATE_HPP___

/* Файл содержит общие определения, используемые для поддержки Lua.
 */

#include "lua.hpp"
#include "lualib.hpp"
#include "stuff.hpp"
#include "reflection_decl.hpp"

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

public:
	State();
	~State();

	/// Получить состояние Lua.
	lua_State* GetState();

	/// Зарегистрировать класс.
	template <typename ClassType>
	void RegisterClass()
	{
		RegisterClass(state, ClassType::scriptClass);
	}

	/// Загрузить скрипт из файла.
	ptr<Script> LoadScript(ptr<File> file);
};

END_INANITY_LUA

#endif
