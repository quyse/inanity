#include "Any.hpp"
#include "State.hpp"
#include "userdata.hpp"
#include "values.ipp"
#include "../../Exception.hpp"

BEGIN_INANITY_LUA

Any::Any(ptr<State> state) : state(state)
{
	lua_State* luaState = state->GetState();

	// pop the value from the stack and store it to registry
	lua_pushlightuserdata(luaState, (void*)&this->state);   // value index
	lua_insert(luaState, lua_absindex(luaState, -2));       // index value
	lua_settable(luaState, LUA_REGISTRYINDEX);
}

Any::~Any()
{
	lua_State* luaState = state->GetState();

	// remove value from registry
	lua_pushlightuserdata(luaState, (void*)&state);
	lua_pushnil(luaState);
	lua_settable(luaState, LUA_REGISTRYINDEX);
}

void Any::PushValue() const
{
	lua_State* luaState = state->GetState();

	// push value into stack
	lua_pushlightuserdata(luaState, (void*)&state);
	lua_gettable(luaState, LUA_REGISTRYINDEX);
}

bool Any::IsNull() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	bool r = !!lua_isnil(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

bool Any::IsNumber() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	bool r = !!lua_isnumber(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

bool Any::IsArray() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	bool r = !!lua_istable(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

bool Any::IsFunction() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	bool r = !!lua_isfunction(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

bool Any::IsObject() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	bool r = !!lua_isuserdata(luaState, -1) && ((UserData*)lua_touserdata(luaState, -1))->type == UserData::typeObject;
	lua_pop(luaState, 1);
	return r;
}

ptr<RefCounted> Any::AsObject() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	ptr<RefCounted> r = Value<ptr<RefCounted> >::Get(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

int Any::AsInt() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	int r = Value<int>::Get(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

float Any::AsFloat() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	float r = Value<float>::Get(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

double Any::AsDouble() const
{
	lua_State* luaState = state->GetState();
	PushValue();
	double r = Value<double>::Get(luaState, -1);
	lua_pop(luaState, 1);
	return r;
}

ptr<Script::Any> Any::CallWith(ptr<Script::Any> arguments[], int count)
{
	return ApplyWith(0, arguments, count);
}

ptr<Script::Any> Any::ApplyWith(ptr<Script::Any> thisValue, ptr<Script::Any> arguments[], int count)
{
	lua_State* luaState = state->GetState();

#ifdef _DEBUG
	/* A special object, which checks that stack is balanced
	after returning from function. Because Run routine works
	in a global stack, that is especially matter. */
	struct StackBalanceCheck
	{
		lua_State* state;
		int top;
		StackBalanceCheck(lua_State* state) : state(state), top(lua_gettop(state)) {}
		~StackBalanceCheck()
		{
			if(top != lua_gettop(state))
				THROW("Unbalanced Lua stack while call Any");
		}
	};
	StackBalanceCheck check(luaState);
#endif

	// push error handler function
	lua_pushcclosure(luaState, ScriptErrorHook, 0);
	// save index of error handler
	int errorHookIndex = lua_gettop(luaState);
	// save current stack
	int beginStack = lua_gettop(luaState);

	// push function into stack
	PushValue();

	// push "this value" if there is
	if(thisValue)
		fast_cast<Any*>(&*thisValue)->PushValue();
	// push arguments into stack
	for(int i = 0; i < count; ++i)
		fast_cast<Any*>(&*arguments[i])->PushValue();

	// perform call
	int code = lua_pcall(luaState, count + (thisValue ? 1 : 0), LUA_MULTRET, errorHookIndex);

	// get amount of results
	int resultsCount = lua_gettop(luaState) - beginStack;

	// remove error handler from stack
	lua_remove(luaState, errorHookIndex);

	// if the call succeeded
	if(code == LUA_OK)
	{
		// if too much values returned, throw
		if(resultsCount > 1)
			THROW("Lua function expected to return no results");

		// if one value is returned
		if(resultsCount == 1)
			// return it (constructor grabs the value from stack)
			return state->CreateAny();
		// else return 0
		else
			return 0;
	}

	// else there is an error on stack, process it
	ProcessError(luaState); // doesn't return

	return 0;
}

ptr<Script::Any> Any::Get(int index) const
{
	lua_State* luaState = state->GetState();
	PushValue();                                      // table
	lua_pushinteger(luaState, (lua_Integer)index);    // table index
	lua_gettable(luaState, -2);                       // table result
	ptr<Any> result = state->CreateAny();             // table
	lua_pop(luaState, 1);                             //
	return result;
}

ptr<Script::Any> Any::Get(ptr<Script::Any> index) const
{
	lua_State* luaState = state->GetState();
	PushValue();                                      // table
	fast_cast<Any*>(&*index)->PushValue();            // table index
	lua_gettable(luaState, -2);                       // table result
	ptr<Any> result = state->CreateAny();             // table
	lua_pop(luaState, 1);                             //
	return result;
}

void Any::Set(int index, ptr<Script::Any> value)
{
	lua_State* luaState = state->GetState();
	PushValue();                                      // table
	lua_pushinteger(luaState, index);                 // table index
	fast_cast<Any*>(&*value)->PushValue();            // table index value
	lua_settable(luaState, -3);                       // table
	lua_pop(luaState, 1);                             //
}

void Any::Set(ptr<Script::Any> index, ptr<Script::Any> value)
{
	lua_State* luaState = state->GetState();
	PushValue();                                      // table
	fast_cast<Any*>(&*index)->PushValue();            // table index
	fast_cast<Any*>(&*value)->PushValue();            // table index value
	lua_settable(luaState, -3);                       // table
	lua_pop(luaState, 1);                             //
}

END_INANITY_LUA
