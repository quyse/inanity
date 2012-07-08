#ifndef ___INANITY_LUA_REFLECTION_HPP___
#define ___INANITY_LUA_REFLECTION_HPP___

#include "stuff.hpp"
#include "lualib.hpp"

BEGIN_INANITY_LUA

/// Абстрактный класс члена класса.
/** Просто, чтобы объединить всё похожее. */
class ClassMember
{
public:
	/// Запихнуть в стек функцию-преобразователь, вызывающий метод.
	virtual void PushThunk(lua_State* luaState) = 0;
};

/// Абстрактный класс именованного члена класса.
class NamedClassMember : public ClassMember
{
private:
	const char* name;

public:
	NamedClassMember(const char* name);

	/// Получить имя метода.
	const char* GetName() const;
};

/// Абстрактный класс, описывающий функцию.
typedef NamedClassMember Function;
/// Абстрактный класс, описывающий метод.
typedef NamedClassMember Method;
/// Абстрактный класс, описывающий конструктор.
typedef ClassMember Constructor;

/// Конкретная функция.
template <typename FunctionType, FunctionType function>
class ConcreteFunction : public Function
{
public:
	ConcreteFunction(const char* name) : Function(name) {}

	void PushThunk(lua_State* luaState)
	{
		lua_pushcclosure(lua_State, &FunctionThunk<FunctionType, function>::Thunk, 0);
	}
};

/// Конкретный метод.
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

/// Конкретный конструктор.
template <typename ClassType, typename... ArgTypes>
class ConcreteConstructor : public Constructor
{
public:
	void PushThunk(lua_State* luaState)
	{
		lua_pushcclosure(lua_State, &ConstructorThunk<ClassType, ArgTypes...>::Thunk, 0);
	}
};

/// Класс, описывающий класс, который можно использовать в скрипте.
class Class
{
private:
	/// Имя класса.
	String name;
	/// Полное имя класса (с namespace).
	String fullName;
	/// Конструктор (если есть).
	Constructor* constructor;
	/// Список методов класса.
	std::vector<Method*> methods;
	/// Список статических методов класса.
	std::vector<Function*> staticMethods;

public:
	Class(const char* fullName);
	~Class();

	/// Указать конструктор.
	template <typename ClassType, typename... ArgTypes>
	void SetConstructor()
	{
		if(constructor)
			delete constructor;
		constructor = new ConcreteConstructor<ClassType, ArgTypes...>();
	}

	/// Добавить метод.
	template <typename MethodType, MethodType method>
	void AddMethod(const char* name)
	{
		methods.push_back(new ConcreteMethod<MethodType, method>(name);
	}

	/// Добавить статический метод.
	template <typename FunctionType, FunctionType function>
	void AddStaticMethod(const char* name)
	{
		staticMethods.push_back(new ConcreteFunction<FunctionType, function>(name));
	}
};

END_INANITY_LUA

#endif
