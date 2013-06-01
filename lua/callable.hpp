#ifndef ___INANITY_LUA_CALLABLE_HPP___
#define ___INANITY_LUA_CALLABLE_HPP___

#include "lua.hpp"

BEGIN_INANITY_LUA

/// Структура, преобразующая константные ссылки в просто данные.
/** Нужна, чтобы можно было хранить такие аргументы в объектах Types
(так как ссылки не могут сами инициализироваться). */
template <typename SourceType>
struct Storagable
{
	typedef SourceType Type;
};
template <typename SourceType>
struct Storagable<const SourceType&>
{
	typedef SourceType Type;
};

/// Структура, хранящая список типов аргументов.
/** Она не может хранить 0 типов, 0 типов - это void.
Если она хранит один тип, то First = этому типу, Rest = void.
Если она хранит больше одного типа, то First = первый тип,
а Rest = Args< оставшиеся типы аналогично >.
*/
template <typename First, typename Rest>
struct Types
{
	typename Storagable<First>::Type first;
	Rest rest;
};
typedef char TypesVoid;
template <typename First>
struct Types<First, void>
{
	typename Storagable<First>::Type first;
	TypesVoid rest; // необходимо, чтобы был хоть какой-нибудь элемент
};

/// Структура, получающая список типов аргументов функции или метода.
/**
Функция или метод определяются просто по типу CalleeType.
Конструктор - это особый случай, это функция с возвращаемым значением типа 
Содержит:
typedef CalleeType - исходный тип функции или метода.
typedef Args - список типов аргументов.
typedef ReturnType - тип возвращаемого значения.
template <CalleeType callee> ReturnType Call(Args& args) - вызов соответствующей функции со списком аргументов.
*/
template <typename CalleeType>
struct Callable;

/// Аналогичная структура для конструкторов.
/** CalleeType = void (ClassType::*)(ArgTypes...) */
template <typename CalleeType>
struct CallableConstructor;

END_INANITY_LUA

#endif
