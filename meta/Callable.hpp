#ifndef ___INANITY_META_CALLABLE_HPP___
#define ___INANITY_META_CALLABLE_HPP___

#include "meta.hpp"

BEGIN_INANITY_META

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

/// Структура, получающая список типов аргументов функции или метода.
/**
Функция или метод определяются просто по типу CalleeType.
Конструктор - это особый случай, это функция с возвращаемым значением типа 
Содержит:
enum { isMethod = 0/1 } - является ли это методом.
typedef CalleeType - исходный тип функции или метода.
typedef Args - список типов аргументов.
typedef ReturnType - тип возвращаемого значения.
typedef ClassType - тип класса (для метода).
template <CalleeType callee> ReturnType Call(Args& args) - вызов соответствующей функции со списком аргументов.
*/
template <typename CalleeType>
struct Callable;

/// Аналогичная структура для конструкторов.
/** CalleeType = void (ClassType::*)(ArgTypes...) */
template <typename CalleeType>
struct CallableConstructor;

END_INANITY_META

#endif
