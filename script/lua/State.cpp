#include "State.hpp"
#include "Function.hpp"
#include "stuff.hpp"
#include "userdata.hpp"
#include "../../meta/Class.hpp"
#include "../../meta/Constructor.hpp"
#include "../../meta/Method.hpp"
#include "../../meta/Function.hpp"
#include "../../File.hpp"
#include "../../Exception.hpp"
#include <cstdlib>
#include <sstream>

BEGIN_INANITY_LUA

State::State()
{
	// создать состояние
	state = lua_newstate(Alloc, this);
	if(!state)
		THROW("Can't create Lua state");

	// установить функцию окончания
	lua_atpanic(state, Panic);
}

State::~State()
{
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

lua_State* State::GetState()
{
	return state;
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
		// конструктор Script заберёт функцию из стека, и сохранит её себе
		return NEW(Function(this));
	// обработать ошибку
	ProcessError(state);
	// ProcessError никогда не возвращает управления
	return 0;
}

void State::Register(Meta::ClassBase* classMeta)
{
	RegisterClassMeta(state, classMeta);
}

END_INANITY_LUA
