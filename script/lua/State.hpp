#ifndef ___INANITY_SCRIPT_LUA_STATE_HPP___
#define ___INANITY_SCRIPT_LUA_STATE_HPP___

/* Файл содержит общие определения, используемые для поддержки Lua.
 */

#include "lua.hpp"
#include "lualib.hpp"
#include "../State.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_META

class ClassBase;

END_INANITY_META

BEGIN_INANITY_LUA

/// The state for Lua interpreter.
class State : public Inanity::Script::State
{
private:
	lua_State* state;

private:
	/// Lua allocation callback.
	static void* Alloc(void* self, void* ptr, size_t osize, size_t nsize);
	/// Lua fatal error callback.
	static int Panic(lua_State* state);

public:
	State();
	~State();

	/// Get internal Lua state.
	lua_State* GetState();

	//*** Script::State methods.
	void Register(Meta::ClassBase* classMeta);
	ptr<Script::Function> LoadScript(ptr<File> file);
	void ReclaimInstance(RefCounted* object);
};

END_INANITY_LUA

#endif
