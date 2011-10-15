#include "LuaState.hpp"
#include "Exception.hpp"
#include <cstdlib>
#include <sstream>

/* Мануал по Lua: http://www.lua.org/manual/5.1/manual.html
 *
 * Как Inanity взаимодействует со скриптом на Lua.
 *
 * Inanity определяет классы, объекты которых могут быть представлены в Lua.
 * Эти классы должны иметь статическую переменную scriptClass, в которой задаётся,
 * какие методы могут быть вызваны этим классом.
 * Объекты таких классов могут быть непосредственно возвращены в скрипт.
 * Классы могут иметь свойства и методы. Свойства - это типа переменных-членов.
 * Их можно (опционально) читать и записывать, используя обыкновенный синтаксис полей Lua:
 * object.field = ...;
 * Методы же вызываются, используя синтаксис Lua для методов:
 * object:method(args...); или object.method(object, args...);
 *
 * Кроме того, классы Inanity могут возвращать делегаты. Делегат - это объект класса плюс
 * метод. В Lua делегат представляется как обычная функция.
 *
 * Реализация скриптовой системы.
 *
 * Объект класса представляется как full userdata, указывающий на структуру ObjectUserData.
 * Кроме того, такому объекту назначается метатаблица global["ObjectMeta"], которая создаётся
 * в момент инициализации скриптовой системы.
 * Делегат представляется как cclosure, с параметрами в замыкании: ObjectUserData и MethodUserData.
 */

LuaState::LuaState()
{
	// создать состояние
	state = lua_newstate(Alloc, this);
	if(!state)
		THROW_PRIMARY_EXCEPTION("Can't create Lua state");

	//******* создать метатаблицу для объектов

	lua_createtable(state, 0, 2);

	// метод для получения данных и методов
	lua_pushlightuserdata(state, this);
	lua_pushcclosure(state, ObjectIndexed, 1);
	lua_setfield(state, -2, "__index");

	// метод-деструктор
	lua_pushlightuserdata(state, this);
	lua_pushcclosure(state, ObjectCollected, 1);
	lua_setfield(state, -2, "__gc");

	// сохранить метатаблицу в реестр
	lua_setglobal(state, "ObjectMeta");
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

/* Indexed вызывается, когда скрипт хочет прочитать свойство, или получить метод.
 * Если свойство, то просто читаем его.
 * Если метод, то возвращается cclosure.
 */
int LuaState::ObjectIndexed(lua_State* state)
{
	// в замыкании лежит состояние
	LuaState* self = (LuaState*)lua_touserdata(state, lua_upvalueindex(1));

	// в стеке лежит: сначала userdata, затем индекс
	// получить userdata объекта
	ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, 1);

	// TODO: сделать свойства; пока только методы

	// получить метод
	lua_getglobal(state, "classes");							// object classes
	lua_pushlightuserdata(state, userData->cls);	// object classes cls
	lua_gettable(state, -2);											// object classes methods
	lua_pushvalue(state, 2);											// object classes methods index
	lua_gettable(state, -2);											// object classes methods method
	lua_replace(state, -3);												// object method methods
	lua_pop(state, 1);														// object method

	// положить в стек делегат
	self->PushDelegate();

	// один ответ возвращён
	return 1;
}

int LuaState::ObjectCollected(lua_State* state)
{
	ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, 1);
	userData->object->Dereference();
	userData->object = 0;
	userData->cls = 0;
	return 0;
}

int LuaState::DelegateCalled(lua_State* state)
{
	// в замыкании лежат: состояние, объект и метод

	// получить состояние
	LuaState* self = (LuaState*)lua_touserdata(state, lua_upvalueindex(1));
	// получить объект
	ObjectUserData* object = (ObjectUserData*)lua_touserdata(state, lua_upvalueindex(2));
	// получить метод
	MethodUserData* method = (MethodUserData*)lua_touserdata(state, lua_upvalueindex(3));

	// вызвать делегат
	return self->CallMethod(object->object, method->method);
}

int LuaState::CallMethod(Object* object, Method method)
{
	// так как мы вызваны из Lua, нельзя кидать исключения
	// мы будем превращать их в ошибки Lua
	try
	{
		Call call(this);
		(object->*method)(call);
		return call.Finish();
	}
	catch(Exception* exception)
	{
		// THINK: неизвестно, будут ли вызываться деструкторы при longjump,
		// вызываемом lua_error, и вообще, корректно ли закроется стековый фрейм
		// поэтому сделаем scope, чтобы хотя бы деструкторы выполнились
		{
			std::ostringstream stream;
			MakePointer(exception)->PrintStack(stream);
			String str = stream.str();
			lua_pushstring(state, str.c_str());
		}
		lua_error(state);
		// на самом деле lua_error не возвращает управления
		return 0;
	}
}

void LuaState::Register(Class* cls)
{
	// проверить, что класс ещё не был зарегистрирован
	lua_getglobal(state, "classes");			// classes
	lua_pushlightuserdata(state, cls);		// classes cls
	lua_gettable(state, -2);							// classes class
	if(lua_istable(state, -1))
	{
		lua_pop(state, 2);									//
		return;
	}

	// здесь мы знаем, что класс не зарегистрирован
	// убрать nil из стека
	lua_pop(state, 1);										// classes
	// зарегистрировать его, то есть создать таблицу со свойствами и методами
	lua_pushlightuserdata(state, cls);		// classes cls
	lua_createtable(state, 0, (int)(cls->getProperties.size() + cls->setProperties.size() + cls->methods.size()));
																				// classes cls class
	// TODO: сделать свойства
	// просто методы
	{
		const std::vector<std::pair<const char*, Method> >& methods = cls->methods;
		for(size_t i = 0; i < methods.size(); ++i)
		{
			lua_pushstring(state, methods[i].first);	// classes cls class methodName
			MethodUserData* methodUserData = (MethodUserData*)lua_newuserdata(state, sizeof(MethodUserData));
																								// classes cls class methodName method
			methodUserData->method = methods[i].second;
			lua_settable(state, -3);									// classes cls class
		}
	}
	// сохранить таблицу
	lua_settable(state, -3);							// classes
	lua_pop(state, 1);										//
}

void LuaState::PushObject(Object* object, Class* cls)
{
	// зарегистрировать класс
	Register(cls);

	// положить в стек объект
	ObjectUserData* userData = (ObjectUserData*)lua_newuserdata(state, sizeof(ObjectUserData));

	// инициализировать
	userData->type = UserData::typeObject;
	userData->object = object;
	userData->cls = cls;

	// увеличить объекту счётчик ссылок
	object->Reference();

	// установить объекту метатаблицу
	lua_getglobal(state, "ObjectMeta");
	lua_setmetatable(state, -2);
}

void LuaState::PushDelegate()
{
	// делегат - это C-замыкание с объектом и методом.
	// на стеке уже должны лежать объект, и имя метода
	// получить метод
	lua_getglobal(state, "classes");						// object methodName classes
	lua_pushvalue(state, -3);										// object methodName classes object
	lua_gettable(state, -2);										// object methodName classes methods
	lua_pushvalue(state, -3);										// object methodName classes methods methodName
	lua_gettable(state, -2);										// object methodName classes methods method
	lua_replace(state, -4);											// object method classes methods
	lua_pop(state, 2);													// object method
	lua_pushcclosure(state, DelegateCalled, 2);	// delegate
}

void LuaState::PushDelegate(Object* object, Class* cls, const char* methodName)
{
	PushObject(object, cls);
	lua_pushstring(state, methodName);
	PushDelegate();
}

//********* класс LuaState::Call

LuaState::Call::Call(LuaState* state) : state(state), returned(false)
{
	// запомнить количество аргументов сразу (на случай, если стек будет меняться)
	argumentsCount = lua_gettop(state->state);
}

int LuaState::Call::GetArgumentsCount() const
{
	return argumentsCount;
}

void LuaState::Call::EnsureArgumentsCount(int count)
{
	if(argumentsCount != count)
		THROW_PRIMARY_EXCEPTION("Invalid arguments count in lua call");
}

bool LuaState::Call::GetBoolean(int i) const
{
	++i;
	if(lua_isboolean(state->state, i))
		return lua_toboolean(state->state, i);
	THROW_PRIMARY_EXCEPTION("Argument is not a boolean");
}

int LuaState::Call::GetInteger(int i) const
{
	++i;
	if(lua_isnumber(state->state, i))
		return (int)lua_tointeger(state->state, i);
	THROW_PRIMARY_EXCEPTION("Argument is not a number");
}

float LuaState::Call::GetFloat(int i) const
{
	++i;
	if(lua_isnumber(state->state, i))
		return (float)lua_tonumber(state->state, i);
	THROW_PRIMARY_EXCEPTION("Argument is not a number");
}

double LuaState::Call::GetDouble(int i) const
{
	++i;
	if(lua_isnumber(state->state, i))
		return (double)lua_tonumber(state->state, i);
	THROW_PRIMARY_EXCEPTION("Argument is not a number");
}

String LuaState::Call::GetString(int i) const
{
	++i;
	if(lua_isstring(state->state, i))
		return lua_tostring(state->state, i);
	THROW_PRIMARY_EXCEPTION("Argument is not a string");
}

template <typename T>
ptr<T> LuaState::Call::GetPointer(int i) const
{
	++i;
	if(lua_isuserdata(state->state, i))
	{
		UserData* userData = (UserData*)lua_touserdata(state->state, i);
		if(userData->type == UserData::typeObject)
		{
			ObjectUserData* objectUserData = (ObjectUserData*)userData;
			return dynamic_cast<T*>(objectUserData->object);
		}
		THROW_PRIMARY_EXCEPTION("Userdata argument is not an object pointer");
	}
	THROW_PRIMARY_EXCEPTION("Argument is not an userdata");
}

void LuaState::Call::CheckReturn()
{
	if(returned)
		lua_pop(state->state, 1);
	else
		returned = true;
}

int LuaState::Call::Finish()
{
	return returned ? 1 : 0;
}

void LuaState::Call::Return(bool value)
{
	CheckReturn();
	lua_pushboolean(state->state, value);
}

void LuaState::Call::Return(int value)
{
	CheckReturn();
	lua_pushinteger(state->state, value);
}

void LuaState::Call::Return(float value)
{
	CheckReturn();
	lua_pushnumber(state->state, value);
}

void LuaState::Call::Return(double value)
{
	CheckReturn();
	lua_pushnumber(state->state, value);
}

void LuaState::Call::Return(const String& value)
{
	CheckReturn();
	lua_pushstring(state->state, value.c_str());
}
