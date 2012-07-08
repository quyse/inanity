#ifndef ___INANITY_LUA_REFLECTION_DECL_HPP___
#define ___INANITY_LUA_REFLECTION_DECL_HPP___

#include "lua.hpp"
#include "lualib.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_LUA

/// Абстрактный класс члена класса.
/** Просто, чтобы объединить всё похожее. */
class ClassMember
{
public:
	virtual ~ClassMember();

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

	void PushThunk(lua_State* luaState);
};

/// Конкретный метод.
template <typename MethodType, MethodType method>
class ConcreteMethod : public Method
{
public:
	ConcreteMethod(const char* name) : Method(name) {}

	void PushThunk(lua_State* luaState);
};

/// Конкретный конструктор.
template <typename ClassType, typename... ArgTypes>
class ConcreteConstructor : public Constructor
{
public:
	void PushThunk(lua_State* luaState);
};

/// Класс, описывающий класс, который можно использовать в скрипте.
class Class
{
private:
	/// Имя класса.
	String name;
	/// Полное имя класса (с namespace).
	String fullName;
public:
	/// Конструктор (если есть).
	Constructor* constructor;
	/// Список методов класса.
	std::vector<Method*> methods;
	/// Список статических методов класса.
	std::vector<Function*> staticMethods;

public:
	Class(const char* fullName);
	~Class();

	/// Получить полное имя класса.
	String GetFullName() const;

	/// Указать конструктор.
	template <typename ClassType, typename... ArgTypes>
	void SetConstructor();

	/// Добавить метод.
	template <typename MethodType, MethodType method>
	void AddMethod(const char* name);

	/// Добавить статический метод.
	template <typename FunctionType, FunctionType function>
	void AddStaticMethod(const char* name);
};

END_INANITY_LUA

#endif
