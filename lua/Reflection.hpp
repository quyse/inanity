#ifndef ___INANITY_LUA_REFLECTION_HPP___
#define ___INANITY_LUA_REFLECTION_HPP___

#include "Calling.hpp"
#include "lualib.hpp"

BEGIN_INANITY_LUA

/// Абстрактный класс, описывающий метод.
/** Конкретные классы получаются из шаблонного класса. */
class Method
{
private:
	const char* name;

public:
	Method(const char* name);

	/// Получить имя метода.
	const char* GetName() const;
	/// Запихнуть в стек функцию-преобразователь, вызывающий метод.
	virtual void PushThunk(lua_State* luaState) = 0;
	/// Запихнуть в стек делегат.
	// пока не реализовано
	//virtual void PushDelegate(lua_State* luaState) = 0;
};

/// Конкретный метод.
/** Шаблонность класса позволяет ему "хранить" тип метода,
и в виртуальную функцию класть шаблонно генерируемую функцию. */
template <typename MethodType, MethodType method>
class ConcreteMethod : public Method
{
public:
	ConcreteMethod(const char* name) : Method(name) {}

	void PushThunk(lua_State* luaState)
	{
		lua_pushcclosure(lua_State, &MethodThunk<MethodType, method>::Thunk, 0);
	}
};

/// Абстрактный класс, описывающий конструктор.
class Constructor
{
public:
	/// Запихнуть в стек функцию-преобразователь, создающую объект.
	virtual void PushThunk(lua_State* luaState) = 0;
};

/// Конкретный конструктор.
template <typename ClassType, typename... ArgTypes>
class ConcreteConstructor : public Constructor
{
public:
	void PushThunk(lua_State* luaState)
	{
//		lua_pushcclosure(lua_State, &ConstructorThunk<
	}
};

/// Класс, описывающий класс, который можно использовать в скрипте.
class Class
{
private:
	/// Конструктор (если есть).
	Constructor* constructor;
	/// Список методов класса.
	std::vector<Method*> methods;

public:
	Class();
	~Class();

	/// Указать конструктор.
	template <typename ClassType, typename... ArgTypes>
	void SetConstructor()
	{
	}

	/// Добавить метод в список методов.
	template <typename MethodType, MethodType method>
	void AddMethod(const char* name)
	{
		methods->push_back(new ConcreteMethod<MethodType, method>(name);
	}
};

END_INANITY_LUA

#endif
