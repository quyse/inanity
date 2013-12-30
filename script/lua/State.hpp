#ifndef ___INANITY_SCRIPT_LUA_STATE_HPP___
#define ___INANITY_SCRIPT_LUA_STATE_HPP___

/* Файл содержит общие определения, используемые для поддержки Lua.
 */

#include "lua.hpp"
#include "MetaProvider.hpp"
#include "lualib.hpp"
#include "../State.hpp"
#include "../../ObjectPool.hpp"
#include <unordered_map>

BEGIN_INANITY_LUA

class Any;

/// The state for Lua interpreter.
class State : public Inanity::Script::State
{
private:
	lua_State* state;

	/// Pool of Any objects.
	ptr<ObjectPool<Any> > anyPool;

	typedef std::unordered_map<lua_State*, State*> States;
	static States states;

private:
	/// Lua allocation callback.
	static void* Alloc(void* self, void* ptr, size_t osize, size_t nsize);
	/// Lua fatal error callback.
	static int Panic(lua_State* state);

	/// Register class by meta.
	void InternalRegister(MetaProvider::ClassBase* classMeta);

public:
	State();
	~State();

	/// Get internal Lua state.
	lua_State* GetState();

	/// Get state by Lua state.
	static ptr<State> GetStateByLuaState(lua_State* state);

	/// Register class.
	template <typename ClassType>
	void Register()
	{
		InternalRegister(Meta::MetaOf<MetaProvider, ClassType>());
	}

	/// Create any value by grabbing one from stack.
	ptr<Any> CreateAny();

	//*** Script::State methods.
	ptr<Script::Function> LoadScript(ptr<File> file);
	void ReclaimInstance(RefCounted* object);
	ptr<Script::Any> NewBoolean(bool boolean);
	ptr<Script::Any> NewNumber(int number);
	ptr<Script::Any> NewNumber(float number);
	ptr<Script::Any> NewNumber(double number);
	ptr<Script::Any> NewString(const String& string);
	ptr<Script::Any> NewArray(int length = 0);
	ptr<Script::Any> NewDict();
};

END_INANITY_LUA

#endif
