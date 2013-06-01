#ifndef ___INANITY_LUA_SCRIPTING_IMPL_HPP___
#define ___INANITY_LUA_SCRIPTING_IMPL_HPP___

#include "reflection.ipp"
#include "callable.ipp"
#include "thunks.ipp"
#include "values.ipp"

// Добавить в файл реализации.
// Начало карты методов.
#define SCRIPTABLE_MAP_BEGIN(className, fullClassName) \
	static void InitConcreteClass_##className(Inanity::Lua::ConcreteClass<className>& cls); \
	Inanity::Lua::ConcreteClass<className> className::scriptClass(#fullClassName, &InitConcreteClass_##className); \
	static void InitConcreteClass_##className(Inanity::Lua::ConcreteClass<className>& cls) {
// Конец карты методов.
#define SCRIPTABLE_MAP_END() }
// Родительский класс.
#define SCRIPTABLE_PARENT(parentClassName) \
	cls.SetParent(&parentClassName::scriptClass)
// Конструктор класса.
#define SCRIPTABLE_CONSTRUCTOR(className, ...) \
	cls.SetConstructor<void (className::*)(__VA_ARGS__)>()
//	cls.SetConstructor<Inanity::Lua::Construct (decltype(cls)::Type::*)(__VA_ARGS__)>()
// Метод в карте методов (статический или нестатический).
#define SCRIPTABLE_METHOD(className, methodName) \
	cls.AddMethod<decltype(&className::methodName), &className::methodName>(#methodName)
//	cls.AddMethod<decltype(&(decltype(cls)::Type)::methodName), &(decltype(cls)::Type)::methodName>(#methodName)

#endif
