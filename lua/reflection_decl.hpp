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
template <typename ConstructorType>
class ConcreteConstructor : public Constructor
{
public:
	void PushThunk(lua_State* luaState);
};

/// Вспомогательная структура для добавления методов.
template <typename CalleeType, CalleeType callee, int isMethod>
struct CalleeAdder;

/// Класс, описывающий класс, который можно использовать в скрипте.
class Class
{
	template <typename CalleeType, CalleeType callee, int isMethod>
	friend struct CalleeAdder;
private:
	/// Имя класса.
	String name;
	/// Полное имя класса (с namespace).
	String fullName;

	/// Родительский класс (если есть).
	/** От него наследуются методы и свойства (нестатические). */
	Class* parent;
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

	/// Получить родительский класс (если есть).
	Class* GetParent() const;
	/// Указать родительский класс.
	void SetParent(Class* parent);

	/// Получить конструктор (если есть).
	Constructor* GetConstructor() const;

	/// Получить методы.
	const std::vector<Method*>& GetMethods() const;
	/// Получить статические методы.
	const std::vector<Function*>& GetStaticMethods() const;

	/// Добавить метод (статический или нестатический).
	template <typename MethodType, MethodType method>
	void AddMethod(const char* name);

	/// Указать конструктор.
	template <typename ConstructorType>
	void SetConstructor();
};

/// Класс конкретного класса.
template <typename ClassType>
class ConcreteClass : public Class
{
public:
	/// Тип исходного класса.
	typedef ClassType Type;

public:
	ConcreteClass(const char* fullName, void (*init)(ConcreteClass<ClassType>&) = 0);
};

END_INANITY_LUA

#endif
