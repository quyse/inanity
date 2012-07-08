#include "LuaState.hpp"
#include "Script.hpp"
#include "../File.hpp"
#include "../Exception.hpp"
#include <cstdlib>
#include <sstream>

State::State()
{
	// создать состояние
	state = lua_newstate(Alloc, this);
	if(!state)
		THROW_PRIMARY_EXCEPTION("Can't create Lua state");
}

LuaState::~LuaState()
{
	lua_close(state);
}

void* LuaState::Alloc(void* self, void* ptr, size_t osize, size_t nsize)
{
	if(nsize)
		return realloc(ptr, nsize);
	free(ptr);
	return 0;
}

ptr<LuaState::Script> LuaState::LoadScript(ptr<File> file)
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
	switch(lua_load(state, Reader::Callback, &reader, "noname", 0))
	{
	case 0:
		// конструктор Script заберёт функцию из стека, и сохранит её себе
		return NEW(Script(this));
	case LUA_ERRSYNTAX:
		THROW_PRIMARY_EXCEPTION("Syntax error in Lua script");
	}
	THROW_PRIMARY_EXCEPTION("Error when loading Lua script");
}
