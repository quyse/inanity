#include "State.hpp"
#include "Any.hpp"
#include "Function.hpp"
#include "stuff.hpp"
#include "values.ipp"
#include "userdata.hpp"
#include "../../File.hpp"
#include "../../Exception.hpp"
#include <cstdlib>
#include <sstream>

BEGIN_INANITY_LUA

State::States State::states;

State::State()
{
	// создать состояние
	state = lua_newstate(Alloc, this);
	if(!state)
		THROW("Can't create Lua state");

	// установить функцию окончания
	lua_atpanic(state, Panic);

	// create pool of Any objects
	anyPool = NEW(ObjectPool<Any>());

	// add state to global list of states
	states.insert(std::make_pair(state, this));
}

State::~State()
{
	states.erase(state);
	lua_close(state);
}

void* State::Alloc(void* self, void* ptr, size_t osize, size_t nsize)
{
	if(nsize)
		return realloc(ptr, nsize);
	free(ptr);
	return 0;
}

int State::Panic(lua_State* state)
{
	try
	{
		ProcessError(state);
		THROW("Unprocessed lua panic");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Lua panic", exception);
	}
}

void State::InternalRegister(MetaProvider::ClassBase* classMeta)
{
	RegisterClassMeta(state, classMeta);
}

lua_State* State::GetState()
{
	return state;
}

ptr<State> State::GetStateByLuaState(lua_State* state)
{
	States::const_iterator i = states.find(state);
	if(i != states.end())
		return i->second;
	return 0;
}

ptr<Any> State::CreateAny()
{
	return anyPool->New(this);
}

ptr<Script::Function> State::LoadScript(ptr<File> file)
{
	/// Класс читателя.
	/** Чтение выполняется в один приём. */
	class Reader
	{
	private:
		ptr<File> file;
		bool read;

	public:
		Reader(ptr<File> file) : file(file), read(false) {}

		static const char* Callback(lua_State* state, void* data, size_t* size)
		{
			Reader* reader = (Reader*)data;
			if(reader->read)
			{
				*size = 0;
				return 0;
			}
			reader->read = true;
			*size = reader->file->GetSize();
			return (const char*)reader->file->GetData();
		};
	};

	Reader reader(file);
	if(lua_load(state, Reader::Callback, &reader, "=noname", 0) == LUA_OK)
		return NEW(Function(CreateAny()));
	// обработать ошибку
	ProcessError(state);
	// ProcessError никогда не возвращает управления
	return 0;
}

void State::ReclaimInstance(RefCounted* object)
{
	ReclaimObject(state, object);
}

ptr<Script::Any> State::NewBoolean(bool boolean)
{
	Value<bool>::Push(state, boolean);
	return CreateAny();
}

ptr<Script::Any> State::NewNumber(int number)
{
	Value<int>::Push(state, number);
	return CreateAny();
}

ptr<Script::Any> State::NewNumber(float number)
{
	Value<float>::Push(state, number);
	return CreateAny();
}

ptr<Script::Any> State::NewNumber(double number)
{
	Value<double>::Push(state, number);
	return CreateAny();
}

ptr<Script::Any> State::NewString(const String& string)
{
	Value<String>::Push(state, string);
	return CreateAny();
}

ptr<Script::Any> State::NewArray(int length)
{
	lua_createtable(state, length, 0);
	return CreateAny();
}

ptr<Script::Any> State::NewDict()
{
	return NewArray(0);
}

END_INANITY_LUA
